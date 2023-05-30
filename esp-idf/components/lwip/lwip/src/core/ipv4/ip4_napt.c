/**
 * @file
 * This is the NAPT implementation on IPv4 layer
 *
 * @see ip4.c
 *
 */
#include "lwip/opt.h"

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * original reassembly code by Adam Dunkels <adam@sics.se>
 *
 */

#if ESP_LWIP && LWIP_IPV4 && IP_NAPT

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "lwip/sys.h"
#include "lwip/ip.h"
#include "lwip/def.h"
#include "lwip/inet_chksum.h"
#include "lwip/netif.h"
#include "lwip/icmp.h"
#include "lwip/udp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/lwip_napt.h"
#include "lwip/ip4_napt.h"
#include "lwip/timeouts.h"
#include "lwip/stats.h"

#define NO_IDX ((u16_t)-1)
#define NT(x) ((x) == NO_IDX ? NULL : &ip_napt_table[x])

#pragma GCC diagnostic push
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wc90-c99-compat" /* To allow u8_t bit fields */
#endif
struct ip_napt_entry {
  u32_t last;
  u32_t src;   /* net */
  u32_t dest;  /* net */
  u16_t sport; /* net */
  u16_t dport; /* net */
  u16_t mport; /* net */
  u8_t proto;
  u8_t fin1 : 1;
  u8_t fin2 : 1;
  u8_t finack1 : 1;
  u8_t finack2 : 1;
  u8_t synack : 1;
  u8_t rst : 1;
  u8_t _unused : 2;
  u32_t src_seqno;  /* host */
  u32_t dest_seqno; /* host */
  u16_t next, prev;
};
#pragma GCC diagnostic pop

struct ip_portmap_entry {
  u32_t maddr;
  u32_t daddr;
  u16_t mport;
  u16_t dport;
  u8_t proto;
  u8_t valid;
};

static u16_t napt_list = NO_IDX, napt_list_last = NO_IDX, napt_free = 0;
static u8_t ip_portmap_max = 0;
static uint16_t ip_napt_max = 0;

static struct ip_napt_entry *ip_napt_table = NULL;
static struct ip_portmap_entry *ip_portmap_table = NULL;

static void ip_napt_gc(uint32_t now, bool force);
static void ip_napt_tmr(void *arg);

#define WRAPPED_AROUND(a, b) ((((a) ^ (b)) & (1UL << 31)) != 0)

#if NAPT_DEBUG
/* Print NAPT table using LWIP_DEBUGF */
#define DPRINTF(m) LWIP_DEBUGF(NAPT_DEBUG, m)
/* #define DPRINTF(m) printf m */
static void
napt_debug_print(void)
{
  int i, next, p;
  u32_t now = sys_now();
#if LWIP_STATS
  u32_t nr_total = STATS_GET(ip_napt.nr_active_tcp) + STATS_GET(ip_napt.nr_active_udp) + STATS_GET(ip_napt.nr_active_icmp);
  DPRINTF(("NAPT table (%"U16_F"+%"U16_F"+%"U16_F"=%"U32_F" / %"U16_F"):\n",
          STATS_GET(ip_napt.nr_active_tcp), STATS_GET(ip_napt.nr_active_udp), STATS_GET(ip_napt.nr_active_icmp), nr_total, ip_napt_max));
  if (nr_total == 0) return;
#endif

  DPRINTF(("+-----------------------+-----------------------+-------+---------+----------+\n"));
  DPRINTF(("| src                   | dest                  | mport | flags   | age      |\n"));
  DPRINTF(("+-----------------------+-----------------------+-------+---------+----------+\n"));
  for (i = napt_list; i != NO_IDX; i = next) {
     struct ip_napt_entry *t = &ip_napt_table[i];
     next = t->next;

     DPRINTF(("| %3"U16_F".%3"U16_F".%3"U16_F".%3"U16_F":%5"U16_F" ",
              ((const u8_t*) (&t->src))[0],
              ((const u8_t*) (&t->src))[1],
              ((const u8_t*) (&t->src))[2],
              ((const u8_t*) (&t->src))[3],
              lwip_ntohs(t->sport)));

     DPRINTF(("| %3"U16_F".%3"U16_F".%3"U16_F".%3"U16_F":%5"U16_F" ",
              ((const u8_t*) (&t->dest))[0],
              ((const u8_t*) (&t->dest))[1],
              ((const u8_t*) (&t->dest))[2],
              ((const u8_t*) (&t->dest))[3],
              lwip_ntohs(t->dport)));

     p = t->proto;
     DPRINTF(("| %5"U16_F" | %c%c%c%c%c%c%c | %8"U32_F" |\n",
              lwip_ntohs(t->mport),
              (p == IP_PROTO_TCP ? 'T' : (p == IP_PROTO_UDP ? 'U' : (p == IP_PROTO_ICMP ? 'I' : '?'))),
              (t->fin1 ? 'f' : '.'),
              (t->fin2 ? 'F' : '.'),
              (t->finack1 ? 'a' : '.'),
              (t->finack2 ? 'A' : '.'),
              (t->synack ? 'S' : '.'),
              (t->rst ? 'R' : '.'),
              now - t->last));

  }
  DPRINTF(("+-----------------------+-----------------------+-------+---------+----------+\n"));
}
#endif /* NAPT_DEBUG */

/**
 * Deallocates the NAPT tables.
 *
 */
static void
ip_napt_deinit(void)
{
  napt_list = NO_IDX;
  ip_napt_max = 0;
  ip_portmap_max = 0;
  mem_free(ip_napt_table);
  ip_napt_table = NULL;
  mem_free(ip_portmap_table);
  ip_portmap_table = NULL;
  sys_untimeout(ip_napt_tmr, NULL);
}

/**
 * Allocates and initializes the NAPT tables.
 *
 * @param max_nat max number of enties in the NAPT table (use IP_NAPT_MAX if in doubt)
 * @param max_portmap max number of enties in the NAPT table (use IP_PORTMAP_MAX if in doubt)
 */
static void
ip_napt_init(uint16_t max_nat, uint8_t max_portmap)
{
  u16_t i;

  if (ip_portmap_table == NULL && ip_napt_table == NULL) {
    ip_napt_table = (struct ip_napt_entry *) mem_calloc(max_nat, sizeof(*ip_napt_table));
    ip_portmap_table = (struct ip_portmap_entry *) mem_calloc(max_portmap, sizeof(*ip_portmap_table));
    assert(ip_portmap_table != NULL && ip_napt_table != NULL);

    for (i = 0; i < max_nat - 1; i++)
      ip_napt_table[i].next = i + 1;
    ip_napt_table[i].next = NO_IDX;
    ip_napt_max = max_nat;
    ip_portmap_max = max_portmap;

    sys_timeout(NAPT_TMR_INTERVAL, ip_napt_tmr, NULL);
  }
}

void
ip_napt_enable(u32_t addr, int enable)
{
  struct netif *netif;
  int napt_in_any_netif = 0;
  for (netif = netif_list; netif; netif = netif->next) {
    if (netif_is_up(netif) && !ip_addr_isany(&netif->ip_addr) && (ip_2_ip4(&netif->ip_addr)->addr) == addr) {
      netif->napt = enable;
    }
    if (netif->napt) {
      napt_in_any_netif = 1;
    }
  }
  if (napt_in_any_netif) {
    ip_napt_init(IP_NAPT_MAX, IP_PORTMAP_MAX);
  } else {
    ip_napt_deinit();
  }
}

void
ip_napt_enable_no(u8_t number, int enable)
{
  struct netif *netif;
  for (netif = netif_list; netif; netif = netif->next) {
    if (netif->num == number) {
      netif->napt = !!enable;
      if (enable)
        ip_napt_init(IP_NAPT_MAX, IP_PORTMAP_MAX);
      else
        ip_napt_deinit();
      break;
    }
  }
}

/* adjusts checksum in a packet
- chksum points to the chksum in the packet
- optr points to the old data in the packet (before)
- nptr points to the new data in the packet (after)
*/
static void
checksumadjust(u8_t *chksum, u8_t *optr, int olen, u8_t *nptr, int nlen)
{
  s32_t x, before, after;
  x=chksum[0]*256+chksum[1];
  x=~x & 0xFFFFU;
  while (olen)
  {
    before=optr[0]*256+optr[1]; optr+=2;
    x-=before & 0xFFFFU;
    if (x<=0) { x--; x&=0xFFFFU; }
    olen-=2;
  }
  while (nlen)
  {
    after=nptr[0]*256+nptr[1]; nptr+=2;
    x+=after & 0xFFFFU;
    if (x & 0x10000U) { x++; x&=0xFFFFU; }
    nlen-=2;
  }
  x=~x & 0xFFFFU;
  chksum[0]=x/256; chksum[1]=x & 0xFFU;
}


static void
ip_napt_send_rst(u32_t src_be, u16_t sport_be, u32_t dst_be, u16_t dport_be, u32_t seqno_le, u32_t ackno_le)
{
  struct pbuf *p = pbuf_alloc(PBUF_IP, TCP_HLEN, PBUF_RAM);
  struct tcp_hdr *tcphdr;
  struct netif *netif;
  ip_addr_t src, dst;
  if (p == NULL) return;
  tcphdr = (struct tcp_hdr *)p->payload;
  tcphdr->src = sport_be;
  tcphdr->dest = dport_be;
  tcphdr->seqno = lwip_htonl(seqno_le);
  tcphdr->ackno = lwip_htonl(ackno_le);
  TCPH_HDRLEN_FLAGS_SET(tcphdr, 5, (TCP_RST | TCP_ACK));
  tcphdr->wnd = lwip_htons(512);
  tcphdr->urgp = 0;
  tcphdr->chksum = 0;
  ip_addr_set_ip4_u32_val(src, src_be);
  ip_addr_set_ip4_u32_val(dst, dst_be);
  tcphdr->chksum = ip_chksum_pseudo(p, IP_PROTO_TCP, p->tot_len, &src, &dst);
  netif = ip4_route(ip_2_ip4(&dst));
  if (netif != NULL) {
    err_t res = ip4_output_if(p, ip_2_ip4(&src), ip_2_ip4(&dst), ICMP_TTL, 0, IP_PROTO_TCP, netif);
    LWIP_UNUSED_ARG(res); /* might be unused if debugging off */
    LWIP_DEBUGF(NAPT_DEBUG, ("SEND RST to %#x:%u from %#x:%u seq %u ack %u res %d\n",
        lwip_ntohl(src_be), lwip_ntohs(sport_be), lwip_ntohl(dst_be), lwip_ntohs(dport_be),
        seqno_le, ackno_le, res));
  }
  pbuf_free(p);
}

/* t must be indexed by napt_free */
static void
ip_napt_insert(struct ip_napt_entry *t)
{
  u16_t ti = t - ip_napt_table;
  assert(ti == napt_free);
  napt_free = t->next;
  t->prev = NO_IDX;
  t->next = napt_list;
  if (napt_list != NO_IDX)
    NT(napt_list)->prev = ti;
  napt_list = ti;
  if (napt_list_last == NO_IDX)
    napt_list_last = ti;

#if LWIP_STATS
#if LWIP_TCP
  if (t->proto == IP_PROTO_TCP)
    STATS_INC(ip_napt.nr_active_tcp);
#endif
#if LWIP_UDP
  if (t->proto == IP_PROTO_UDP)
    STATS_INC(ip_napt.nr_active_udp);
#endif
#if LWIP_ICMP
  if (t->proto == IP_PROTO_ICMP)
    STATS_INC(ip_napt.nr_active_icmp);
#endif
  LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_insert(): TCP=%d, UDP=%d, ICMP=%d\n",
          STATS_GET(ip_napt.nr_active_tcp), STATS_GET(ip_napt.nr_active_udp), STATS_GET(ip_napt.nr_active_icmp)));
#endif /* LWIP_STATS */
}

static void
ip_napt_free(struct ip_napt_entry *t)
{
  u16_t ti = t - ip_napt_table;
  if (ti == napt_list)
    napt_list = t->next;
  if (ti == napt_list_last)
    napt_list_last = t->prev;
  if (t->next != NO_IDX)
    NT(t->next)->prev = t->prev;
  if (t->prev != NO_IDX)
    NT(t->prev)->next = t->next;
  t->prev = NO_IDX;
  t->next = napt_free;
  napt_free = ti;

#if LWIP_STATS
#if LWIP_TCP
  if (t->proto == IP_PROTO_TCP)
    STATS_DEC(ip_napt.nr_active_tcp);
#endif
#if LWIP_UDP
  if (t->proto == IP_PROTO_UDP)
    STATS_DEC(ip_napt.nr_active_udp);
#endif
#if LWIP_ICMP
  if (t->proto == IP_PROTO_ICMP)
    STATS_DEC(ip_napt.nr_active_icmp);
#endif
#endif /* LWIP_STATS */

  LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_free\n"));
#if NAPT_DEBUG
  napt_debug_print();
#endif
  /* Send RST to both sides to let them know connection is being evicted */
  if (t->proto == IP_PROTO_TCP && t->synack && !(t->fin1 || t->fin2 || t-> rst)) {
    /* Send RST both ways. */
    ip_napt_send_rst(t->dest, t->dport, t->src, t->sport, t->dest_seqno, t->src_seqno);
    ip_napt_send_rst(t->src, t->sport, t->dest, t->dport, t->src_seqno, t->dest_seqno);
  }
}

#if LWIP_TCP
static u8_t
ip_napt_find_port(u8_t proto, u16_t port)
{
  int i, next;
  for (i = napt_list; i != NO_IDX; i = next) {
    struct ip_napt_entry *t = &ip_napt_table[i];
    next = t->next;
    if (t->proto == proto && t->mport == port)
      return 1;
  }
  return 0;
}

static struct ip_portmap_entry *
ip_portmap_find(u8_t proto, u16_t mport);

static u8_t
tcp_listening(u16_t port)
{
  struct tcp_pcb_listen *t;
  for (t = tcp_listen_pcbs.listen_pcbs; t; t = t->next)
    if (t->local_port == port)
      return 1;
  if (ip_portmap_find(IP_PROTO_TCP, port))
    return 1;
  return 0;
}
#endif /* LWIP_TCP */

#if LWIP_UDP
static u8_t
udp_listening(u16_t port)
{
  struct udp_pcb *pcb;
  for (pcb = udp_pcbs; pcb; pcb = pcb->next)
    if (pcb->local_port == port)
      return 1;
  if (ip_portmap_find(IP_PROTO_UDP, port))
    return 1;
  return 0;
}
#endif /* LWIP_UDP */

static u16_t
ip_napt_new_port(u8_t proto, u16_t port)
{
  if (PP_NTOHS(port) >= IP_NAPT_PORT_RANGE_START && PP_NTOHS(port) <= IP_NAPT_PORT_RANGE_END)
    if (!ip_napt_find_port(proto, port) && !tcp_listening(port))
      return port;
  for (;;) {
    port = PP_HTONS(IP_NAPT_PORT_RANGE_START +
                    LWIP_RAND() % (IP_NAPT_PORT_RANGE_END - IP_NAPT_PORT_RANGE_START + 1));
    if (ip_napt_find_port(proto, port))
      continue;
#if LWIP_TCP
    if (proto == IP_PROTO_TCP && tcp_listening(port))
      continue;
#endif /* LWIP_TCP */
#if LWIP_UDP
    if (proto == IP_PROTO_UDP && udp_listening(port))
      continue;
#endif /* LWIP_UDP */

    return port;
  }
}

static struct ip_napt_entry*
ip_napt_find(u8_t proto, u32_t addr, u16_t port, u16_t mport, u8_t dest)
{
  u16_t i, next;
  u32_t now;
  struct ip_napt_entry *t;

  LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_find\n"));
  LWIP_DEBUGF(NAPT_DEBUG, ("looking up in table %s: %"U16_F".%"U16_F".%"U16_F".%"U16_F", port: %u, mport: %u\n",
                           (dest ? "dest" : "src"),
                           ((const u8_t*) (&addr))[0], ((const u8_t*) (&addr))[1],
                           ((const u8_t*) (&addr))[2], ((const u8_t*) (&addr))[3],
                           PP_HTONS(port),
                           PP_HTONS(mport)));
#if NAPT_DEBUG
  napt_debug_print();
#endif

  now = sys_now();
  for (i = napt_list; i != NO_IDX; i = next) {
    t = NT(i);
    next = t->next;
    if (!dest && t->proto == proto && t->src == addr && t->sport == port) {
      t->last = now;
      LWIP_DEBUGF(NAPT_DEBUG, ("found\n"));
      return t;
    }
    if (dest && t->proto == proto && t->dest == addr && t->dport == port
        && t->mport == mport) {
      t->last = now;
      LWIP_DEBUGF(NAPT_DEBUG, ("found\n"));
      return t;
    }
  }

  LWIP_DEBUGF(NAPT_DEBUG, ("not found\n"));
  return NULL;
}

static u16_t
ip_napt_add(u8_t proto, u32_t src, u16_t sport, u32_t dest, u16_t dport, u32_t seqno)
{
  struct ip_napt_entry *t = ip_napt_find(proto, src, sport, 0, 0);
  if (t) {
    t->last = sys_now();
    t->dest = dest;
    t->dport = dport;
    /* move this entry to the top of napt_list */
    ip_napt_free(t);
    ip_napt_insert(t);

    LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_add\n"));
#if NAPT_DEBUG
    napt_debug_print();
#endif

    return t->mport;
  }
  t = NT(napt_free);
  if (!t) {
    ip_napt_gc(sys_now(), true /* make_room */);
    t = NT(napt_free);
  }
  if (t) {
    u16_t mport = sport;
#if LWIP_TCP
    if (proto == IP_PROTO_TCP)
      mport = ip_napt_new_port(IP_PROTO_TCP, sport);
#endif
#if LWIP_TCP
    if (proto == IP_PROTO_UDP)
      mport = ip_napt_new_port(IP_PROTO_UDP, sport);
#endif
    t->last = sys_now();
    t->src = src;
    t->dest = dest;
    t->sport = sport;
    t->dport = dport;
    t->mport = mport;
    t->proto = proto;
    t->fin1 = t->fin2 = t->finack1 = t->finack2 = t->synack = t->rst = 0;
    t->src_seqno = ntohl(seqno);
    t->dest_seqno = 0;
    ip_napt_insert(t);

    LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_add\n"));
#if NAPT_DEBUG
    napt_debug_print();
#endif

    return mport;
  }
  LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_add() failed to insert\n"));
  return 0;
}

u8_t
ip_portmap_add(u8_t proto, u32_t maddr, u16_t mport, u32_t daddr, u16_t dport)
{
  int i;
  mport = PP_HTONS(mport);
  dport = PP_HTONS(dport);

  for (i = 0; i < ip_portmap_max; i++) {
    struct ip_portmap_entry *p = &ip_portmap_table[i];
    if (p->valid && p->proto == proto && p->mport == mport) {
      p->dport = dport;
      p->daddr = daddr;
    } else if (!p->valid) {
      p->maddr = maddr;
      p->daddr = daddr;
      p->mport = mport;
      p->dport = dport;
      p->proto = proto;
      p->valid = 1;
      return 1;
    }
  }
  return 0;
}

static struct ip_portmap_entry *
ip_portmap_find(u8_t proto, u16_t mport)
{
  int i;
  for (i = 0; i < ip_portmap_max; i++) {
    struct ip_portmap_entry *p = &ip_portmap_table[i];
    if (!p->valid)
      return 0;
    if (p->proto == proto && p->mport == mport)
      return p;
  }
  return NULL;
}

static struct ip_portmap_entry *
ip_portmap_find_dest(u8_t proto, u16_t dport, u32_t daddr)
{
  int i;
  for (i = 0; i < ip_portmap_max; i++) {
    struct ip_portmap_entry *p = &ip_portmap_table[i];
    if (!p->valid)
      return 0;
    if (p->proto == proto && p->dport == dport && p->daddr == daddr)
      return p;
  }
  return NULL;
}

u8_t
ip_portmap_get(u8_t proto, u16_t mport, u32_t *maddr, u32_t *daddr, u16_t *dport)
{
  struct ip_portmap_entry *m = ip_portmap_find(proto, PP_HTONS(mport));
  if (!m)
    return 0;
  *maddr = m->maddr;
  *daddr = m->daddr;
  *dport = PP_NTOHS(m->dport);
  return 1;
}

u8_t
ip_portmap_remove(u8_t proto, u16_t mport)
{
  struct ip_portmap_entry *last = &ip_portmap_table[ip_portmap_max - 1];
  struct ip_portmap_entry *m = ip_portmap_find(proto, PP_HTONS(mport));
  if (!m)
    return 0;
  for (; m != last; m++)
    memcpy(m, m + 1, sizeof(*m));
  last->valid = 0;
  return 1;
}

#if LWIP_TCP
static void
ip_napt_modify_port_tcp(struct tcp_hdr *tcphdr, u8_t dest, u16_t newval)
{
  if (dest) {
    checksumadjust((u8_t *)&tcphdr->chksum, (u8_t *)&tcphdr->dest, 2, (u8_t *)&newval, 2);
    tcphdr->dest = newval;
  } else {
    checksumadjust((u8_t *)&tcphdr->chksum, (u8_t *)&tcphdr->src, 2, (u8_t *)&newval, 2);
    tcphdr->src = newval;
  }
}

static void
ip_napt_modify_addr_tcp(struct tcp_hdr *tcphdr, ip4_addr_p_t *oldval, u32_t newval)
{
  checksumadjust((u8_t *)&tcphdr->chksum, (u8_t *)&oldval->addr, 4, (u8_t *)&newval, 4);
}
#endif /* LWIP_TCP */

#if LWIP_UDP
static void
ip_napt_modify_port_udp(struct udp_hdr *udphdr, u8_t dest, u16_t newval)
{
  if (dest) {
    if (udphdr->chksum != 0)
      checksumadjust((u8_t *)&udphdr->chksum, (u8_t *)&udphdr->dest, 2, (u8_t *)&newval, 2);
    udphdr->dest = newval;
  } else {
    if (udphdr->chksum != 0)
      checksumadjust((u8_t *)&udphdr->chksum, (u8_t *)&udphdr->src, 2, (u8_t *)&newval, 2);
    udphdr->src = newval;
  }
}

static void
ip_napt_modify_addr_udp(struct udp_hdr *udphdr, ip4_addr_p_t *oldval, u32_t newval)
{
  if (udphdr->chksum != 0)
    checksumadjust( (u8_t *)&udphdr->chksum, (u8_t *)&oldval->addr, 4, (u8_t *)&newval, 4);
}
#endif /* LWIP_UDP */

static void
ip_napt_modify_addr(struct ip_hdr *iphdr, ip4_addr_p_t *field, u32_t newval)
{
  checksumadjust((u8_t *)&IPH_CHKSUM(iphdr), (u8_t *)&field->addr, 4, (u8_t *)&newval, 4);
  field->addr = newval;
}

void
ip_napt_recv(struct pbuf *p, struct ip_hdr *iphdr)
{
  struct ip_portmap_entry *m;
  struct ip_napt_entry *t;

  if (ip_napt_max == 0) return;

#if LWIP_ICMP
  /* NAPT for ICMP Echo Request using identifier */
  if (IPH_PROTO(iphdr) == IP_PROTO_ICMP) {
    struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);
    if (iecho->type == ICMP_ER) {
      t = ip_napt_find(IP_PROTO_ICMP, iphdr->src.addr, iecho->id, iecho->id, 1);
      if (!t)
        return;
      ip_napt_modify_addr(iphdr, &iphdr->dest, t->src);
      return;
    }

    return;
  }
#endif /* LWIP_ICMP */

#if LWIP_TCP
  if (IPH_PROTO(iphdr) == IP_PROTO_TCP) {
    uint32_t seqno, dest_seqno;
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);


    LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_recv\n"));
    LWIP_DEBUGF(NAPT_DEBUG, ("src: %"U16_F".%"U16_F".%"U16_F".%"U16_F", dest: %"U16_F".%"U16_F".%"U16_F".%"U16_F", \n",
                             ip4_addr1_16(&iphdr->src), ip4_addr2_16(&iphdr->src),
                             ip4_addr3_16(&iphdr->src), ip4_addr4_16(&iphdr->src),
                             ip4_addr1_16(&iphdr->dest), ip4_addr2_16(&iphdr->dest),
                             ip4_addr3_16(&iphdr->dest), ip4_addr4_16(&iphdr->dest)));

    LWIP_DEBUGF(NAPT_DEBUG, ("sport %u, dport: %u\n",
                             lwip_htons(tcphdr->src),
                             lwip_htons(tcphdr->dest)));

    m = ip_portmap_find(IP_PROTO_TCP, tcphdr->dest);
    if (m) {
      /* packet to mapped port: rewrite destination */
      if (m->dport != tcphdr->dest)
        ip_napt_modify_port_tcp(tcphdr, 1, m->dport);
      ip_napt_modify_addr_tcp(tcphdr, &iphdr->dest, m->daddr);
      ip_napt_modify_addr(iphdr, &iphdr->dest, m->daddr);
      return;
    }
    t = ip_napt_find(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, tcphdr->dest, 1);
    if (!t)
      return; /* Unknown TCP session; do nothing */

    if (t->sport != tcphdr->dest)
      ip_napt_modify_port_tcp(tcphdr, 1, t->sport);
    ip_napt_modify_addr_tcp(tcphdr, &iphdr->dest, t->src);
    ip_napt_modify_addr(iphdr, &iphdr->dest, t->src);

    if ((TCPH_FLAGS(tcphdr) & (TCP_SYN|TCP_ACK)) == (TCP_SYN|TCP_ACK))
      t->synack = 1;
    if ((TCPH_FLAGS(tcphdr) & TCP_FIN))
      t->fin1 = 1;
    if (t->fin2 && (TCPH_FLAGS(tcphdr) & TCP_ACK))
      t->finack2 = 1; /* FIXME: Currently ignoring ACK seq... */
    if (TCPH_FLAGS(tcphdr) & TCP_RST)
      t->rst = 1;
    seqno = ntohl(tcphdr->seqno);
    dest_seqno = t->dest_seqno;
    if (seqno >= dest_seqno || WRAPPED_AROUND(seqno, dest_seqno)) {
      t->dest_seqno = seqno + (p->tot_len - IPH_HL(iphdr) * 4 - TCPH_HDRLEN_BYTES(tcphdr));
    }
    return;
  }
#endif /* LWIP_TCP */

#if LWIP_UDP
  if (IPH_PROTO(iphdr) == IP_PROTO_UDP) {
    struct udp_hdr *udphdr = (struct udp_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);
    m = ip_portmap_find(IP_PROTO_UDP, udphdr->dest);
    if (m) {
      /* packet to mapped port: rewrite destination */
      if (m->dport != udphdr->dest)
        ip_napt_modify_port_udp(udphdr, 1, m->dport);
      ip_napt_modify_addr_udp(udphdr, &iphdr->dest, m->daddr);
      ip_napt_modify_addr(iphdr, &iphdr->dest, m->daddr);
      return;
    }
    t = ip_napt_find(IP_PROTO_UDP, iphdr->src.addr, udphdr->src, udphdr->dest, 1);
      if (!t)
        return; /* Unknown session; do nothing */

      if (t->sport != udphdr->dest)
        ip_napt_modify_port_udp(udphdr, 1, t->sport);
      ip_napt_modify_addr_udp(udphdr, &iphdr->dest, t->src);
      ip_napt_modify_addr(iphdr, &iphdr->dest, t->src);
      return;
  }
#endif /* LWIP_UDP */
}

err_t
ip_napt_forward(struct pbuf *p, struct ip_hdr *iphdr, struct netif *inp, struct netif *outp)
{
  if (!inp->napt)
    return ERR_OK;

#if LWIP_ICMP
  /* NAPT for ICMP Echo Request using identifier */
  if (IPH_PROTO(iphdr) == IP_PROTO_ICMP) {
    struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);
    if (iecho->type == ICMP_ECHO) {
      /* register src addr and iecho->id and dest info */
      ip_napt_add(IP_PROTO_ICMP, iphdr->src.addr, iecho->id, iphdr->dest.addr, iecho->id, 0);

      ip_napt_modify_addr(iphdr, &iphdr->src, ip_2_ip4(&outp->ip_addr)->addr);
    }

    return ERR_OK;
  }
#endif

#if LWIP_TCP
  if (IPH_PROTO(iphdr) == IP_PROTO_TCP) {
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);
    u16_t mport;

    struct ip_portmap_entry *m = ip_portmap_find_dest(IP_PROTO_TCP, tcphdr->src, iphdr->src.addr);
    if (m) {
      /* packet from port-mapped dest addr/port: rewrite source to this node */
      if (m->mport != tcphdr->src)
        ip_napt_modify_port_tcp(tcphdr, 0, m->mport);
      ip_napt_modify_addr_tcp(tcphdr, &iphdr->src, m->maddr);
      ip_napt_modify_addr(iphdr, &iphdr->src, m->maddr);
      return ERR_OK;
    }
    if ((TCPH_FLAGS(tcphdr) & (TCP_SYN|TCP_ACK)) == TCP_SYN &&
        PP_NTOHS(tcphdr->src) >= 1024) {
      /* Register new TCP session to NAPT */
      mport = ip_napt_add(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src,
                          iphdr->dest.addr, tcphdr->dest, tcphdr->seqno);
      if (mport == 0) {
#if LWIP_ICMP
        icmp_dest_unreach(p, ICMP_DUR_PORT);
#endif
        return ERR_RTE; /* routing err if add entry failed */
      }
    } else {
      uint32_t seqno, src_seqno;
      struct ip_napt_entry *t = ip_napt_find(IP_PROTO_TCP, iphdr->src.addr, tcphdr->src, 0, 0);
      if (!t || t->dest != iphdr->dest.addr || t->dport != tcphdr->dest) {
#if LWIP_ICMP
        icmp_dest_unreach(p, ICMP_DUR_PORT);
#endif
        return ERR_RTE; /* Drop unknown TCP session */
      }
      mport = t->mport;
      if ((TCPH_FLAGS(tcphdr) & TCP_FIN))
        t->fin2 = 1;
      if (t->fin1 && (TCPH_FLAGS(tcphdr) & TCP_ACK))
        t->finack1 = 1; /* FIXME: Currently ignoring ACK seq... */
      if (TCPH_FLAGS(tcphdr) & TCP_RST)
        t->rst = 1;
      seqno = ntohl(tcphdr->seqno);
      src_seqno = t->src_seqno;
      if (seqno >= t->src_seqno || WRAPPED_AROUND(seqno, src_seqno)) {
        t->src_seqno = seqno + (p->tot_len - IPH_HL(iphdr) * 4 - TCPH_HDRLEN_BYTES(tcphdr));
      }
    }

    if (mport != tcphdr->src)
      ip_napt_modify_port_tcp(tcphdr, 0, mport);
    ip_napt_modify_addr_tcp(tcphdr, &iphdr->src, ip_2_ip4(&outp->ip_addr)->addr);
    ip_napt_modify_addr(iphdr, &iphdr->src, ip_2_ip4(&outp->ip_addr)->addr);

    return ERR_OK;
  }
#endif

#if LWIP_UDP
  if (IPH_PROTO(iphdr) == IP_PROTO_UDP) {
    struct udp_hdr *udphdr = (struct udp_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);
    u16_t mport;

    struct ip_portmap_entry *m = ip_portmap_find_dest(IP_PROTO_UDP, udphdr->src, iphdr->src.addr);
    if (m) {
      /* packet from port-mapped dest addr/port: rewrite source to this node */
      if (m->mport != udphdr->src)
        ip_napt_modify_port_udp(udphdr, 0, m->mport);
      ip_napt_modify_addr_udp(udphdr, &iphdr->src, m->maddr);
      LWIP_DEBUGF(NAPT_DEBUG, ("Modify UDP addr %x %x", iphdr->src.addr, m->maddr));
      ip_napt_modify_addr(iphdr, &iphdr->src, m->maddr);
      return ERR_OK;
    }
    if (PP_NTOHS(udphdr->src) >= 1024) {
      /* Register new UDP session */
      mport = ip_napt_add(IP_PROTO_UDP, iphdr->src.addr, udphdr->src,
                          iphdr->dest.addr, udphdr->dest, 0);
      if (mport == 0)
        return ERR_RTE; /* routing err if add entry failed */
    } else {
      struct ip_napt_entry *t = ip_napt_find(IP_PROTO_UDP, iphdr->src.addr, udphdr->src, 0, 0);
      if (!t || t->dest != iphdr->dest.addr || t->dport != udphdr->dest) {
#if LWIP_ICMP
        icmp_dest_unreach(p, ICMP_DUR_PORT);
#endif
        return ERR_RTE; /* Drop unknown UDP session */
      }
      mport = t->mport;
    }

    if (mport != udphdr->src)
      ip_napt_modify_port_udp(udphdr, 0, mport);
    ip_napt_modify_addr_udp(udphdr, &iphdr->src, ip_2_ip4(&outp->ip_addr)->addr);
    ip_napt_modify_addr(iphdr, &iphdr->src, ip_2_ip4(&outp->ip_addr)->addr);
    return ERR_OK;
  }
#endif

  return ERR_OK;
}

static void
ip_napt_gc(uint32_t now, bool force)
{
  u16_t i, next, oldest = NO_IDX;
  u32_t age = 0, oldest_age = 0;
  int checked = 0, evicted = 0, forced = 0;
  for (i = napt_list; i != NO_IDX; i = next) {
    struct ip_napt_entry *t = &ip_napt_table[i];
    checked++;
    next = t->next;
    age = now - t->last;
    if (age > oldest_age) {
      oldest = i;
      oldest_age = age;
    }
#if LWIP_TCP
    if (t->proto == IP_PROTO_TCP) {
      if (age > IP_NAPT_TIMEOUT_MS_TCP_DISCON) {
        if ((t->finack1 || t->finack2 || !t->synack || t->rst) ||
            age > IP_NAPT_TIMEOUT_MS_TCP) {
          ip_napt_free(t);
          evicted++;
          if (force) break;
        }
      }
      continue;
    }
#endif
#if LWIP_UDP
    if (t->proto == IP_PROTO_UDP) {
      if (age > IP_NAPT_TIMEOUT_MS_UDP) {
        ip_napt_free(t);
        evicted++;
        if (force) break;
      }
      continue;
    }
#endif
#if LWIP_ICMP
    if (t->proto == IP_PROTO_ICMP) {
      if (age > IP_NAPT_TIMEOUT_MS_ICMP) {
        ip_napt_free(t);
        evicted++;
        if (force) break;
      }
      continue;
    }
#endif
  }
  if (napt_free == NO_IDX && force && oldest != NO_IDX) {
    ip_napt_free(&ip_napt_table[oldest]);
    evicted++;
    forced++;
    STATS_INC(ip_napt.nr_forced_evictions);
  }
  LWIP_DEBUGF(NAPT_DEBUG, ("ip_napt_gc(%d): chk %d evict %d (forced %d), oldest %u\n",
                           force, checked, evicted, forced, oldest_age));
}

static void
ip_napt_maint(void)
{
  static uint32_t s_last_now = 0;
  uint32_t now;
  if (napt_list == NO_IDX) return;
  now = sys_now();
  /* Check for timestamp wraparound (happens every ~49.7 days). */
  if (WRAPPED_AROUND(s_last_now, now)) {
    u16_t i;
    struct ip_napt_entry *t;
    for (i = napt_list; i != NO_IDX; i = t->next) {
      t = &ip_napt_table[i];
      /* It's a very simplistic way of dealing with it
       * but it's fine for our purposes. */
      t->last = now;
    }
  }
  ip_napt_gc(now, false /* make_room */);
  s_last_now = now;
}

static void
ip_napt_tmr(void *arg)
{
  ip_napt_maint();
  sys_timeout(NAPT_TMR_INTERVAL, ip_napt_tmr, arg);
}

#if LWIP_STATS
void
ip_napt_get_stats(struct stats_ip_napt *stats)
{
  *stats = STATS_GET(ip_napt);
}
#endif /* LWIP_STATS */

#endif /* ESP_LWIP && LWIP_IPV4 && IP_NAPT */
