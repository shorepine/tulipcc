// multicast.c
#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "lwip/netdb.h"

#include "alles.h"

static const char *TAG = "multicast";
static const char *V4TAG = "mcast-ipv4";

int sock= -1;

extern void deserialize_event(char * message, uint16_t length);
uint8_t ipv4_quartet;
char udp_message[MAX_RECEIVE_LEN];
extern char *message_start_pointer;
extern int16_t message_length;
extern void delay_ms(uint32_t ms);
uint32_t udp_message_counter = 0;



int64_t last_ping_time = PING_TIME_MS; // do the first ping at 10s in to wait for other synths to announce themselves

static int socket_add_ipv4_multicast_group(bool assign_source_if) {
    struct ip_mreq imreq = { 0 };
    struct in_addr iaddr = { 0 };
    int err = 0;
    // Configure source interface
    tcpip_adapter_ip_info_t ip_info = { 0 };
    tcpip_adapter_get_ip_info(WIFI_IF_STA, &ip_info);
    inet_addr_from_ip4addr(&iaddr, &ip_info.ip);
    // Configure multicast address to listen to
    err = inet_aton(MULTICAST_IPV4_ADDR, &imreq.imr_multiaddr.s_addr);
    if (err != 1) {
        ESP_LOGE(V4TAG, "Configured IPV4 multicast address '%s' is invalid.", MULTICAST_IPV4_ADDR);
        // Errors in the return value have to be negative
        err = -1;
        goto err;
    }
    ESP_LOGI(TAG, "Configured IPV4 Multicast address %s", inet_ntoa(imreq.imr_multiaddr.s_addr));
    if (!IP_MULTICAST(ntohl(imreq.imr_multiaddr.s_addr))) {
        ESP_LOGW(V4TAG, "Configured IPV4 multicast address '%s' is not a valid multicast address. This will probably not work.", MULTICAST_IPV4_ADDR);
    }

    if (assign_source_if) {
        // Assign the IPv4 multicast source interface, via its IP
        // (only necessary if this socket is IPV4 only)
        err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
                         sizeof(struct in_addr));
        if (err < 0) {
            ESP_LOGE(V4TAG, "Failed to set IP_MULTICAST_IF. Error %d", errno);
            goto err;
        }
    }

    err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                         &imreq, sizeof(struct ip_mreq));
    if (err < 0) {
        ESP_LOGE(V4TAG, "Failed to set IP_ADD_MEMBERSHIP. Error %d", errno);
        goto err;
    }

 err:
    return err;
}

void create_multicast_ipv4_socket(void) {
    struct sockaddr_in saddr = { 0 };
    sock = -1;
    int err = 0;
    fprintf(stderr, "sock\n");
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(V4TAG, "Failed to create socket. Error %d", errno);
    }

    // Bind the socket to any address
    saddr.sin_family = PF_INET;
    saddr.sin_port = htons(UDP_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (err < 0) {
        ESP_LOGE(V4TAG, "Failed to bind socket. Error %d", errno);
    }

    // Assign multicast TTL (set separately from normal interface TTL)
    uint8_t ttl = MULTICAST_TTL;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(uint8_t));
    if (err < 0) {
        ESP_LOGE(V4TAG, "Failed to set IP_MULTICAST_TTL. Error %d", errno);
    }

    uint8_t loopback_val = 1;
    err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
                     &loopback_val, sizeof(uint8_t));
    if (err < 0) {
        ESP_LOGE(V4TAG, "Failed to set IP_MULTICAST_LOOP. Error %d", errno);
    }

    // this is also a listening socket, so add it to the multicast
    // group for listening...
    err = socket_add_ipv4_multicast_group(true);

    // All set, socket is configured for sending and receiving
}

// Send a multicast message 
void mcast_send(char * message, uint16_t len) {
    char addrbuf[32] = { 0 };
    struct addrinfo hints = {
        .ai_flags = AI_PASSIVE,
        .ai_socktype = SOCK_DGRAM,
    };
    struct addrinfo *res;


    hints.ai_family = AF_INET; // For an IPv4 socket
    int err = getaddrinfo(MULTICAST_IPV4_ADDR, NULL, &hints, &res);
    if (err < 0) {
        ESP_LOGE(TAG, "getaddrinfo() failed for IPV4 destination address. error: %d", err);
    }
    if (res == 0) {
        ESP_LOGE(TAG, "getaddrinfo() did not return any addresses");
    }
    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(UDP_PORT);
    inet_ntoa_r(((struct sockaddr_in *)res->ai_addr)->sin_addr, addrbuf, sizeof(addrbuf)-1);
    //ESP_LOGI(TAG, "Sending to IPV4 multicast address %s:%d...",  addrbuf, UDP_PORT);
    //printf("sending message %s\n", message);
    err = sendto(sock, message, len, 0, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if (err < 0) {
        ESP_LOGE(TAG, "IPV4 sendto failed. errno: %d", errno);
    }
}



void mcast_listen_task(void *pvParameters) {
    struct timeval tv = {
        .tv_sec =  1,
        .tv_usec = 0,
    };
    
    //ipv4_quartet = esp_ip4_addr4(&wifi_manager_ip4);
    tcpip_adapter_ip_info_t ip_info = { 0 };
    tcpip_adapter_get_ip_info(WIFI_IF_STA, &ip_info);
    ipv4_quartet = ip4_addr4(&ip_info.ip);

    int16_t full_message_length;
    printf("Network listening running on core %d\n",xPortGetCoreID());
    while (1) {

        if (sock < 0) ESP_LOGE(TAG, "Failed to create IPv4 multicast socket");
        // set destination multicast addresses for sending from these sockets
        struct sockaddr_in sdestv4 = {
            .sin_family = PF_INET,
            .sin_port = htons(UDP_PORT),
        };
        // We know this inet_aton will pass because we did it above already
        inet_aton(MULTICAST_IPV4_ADDR, &sdestv4.sin_addr.s_addr);

        int err = 1;
        while (err > 0) { 
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(sock, &rfds);

            int s = select(sock + 1, &rfds, NULL, NULL, &tv);
            if (s < 0) {
                ESP_LOGE(TAG, "Select failed: errno %d", errno);
                err = -1;
                break;
            }
            else if (s > 0) {
                if (FD_ISSET(sock, &rfds)) {
                    // Incoming UDP packet received
                    // Turn on the CPU monitor to see how long parsing takes
                    struct sockaddr_in6 raddr; // Large enough for both IPv4 or IPv6
                    socklen_t socklen = sizeof(raddr);
                    full_message_length = recvfrom(sock, udp_message, sizeof(udp_message)-1, 0,
                                       (struct sockaddr *)&raddr, &socklen);
                    if (full_message_length < 0) {
                        ESP_LOGE(TAG, "multicast recvfrom failed: errno %d", errno);
                        err = -1;
                        break;
                    }
                    udp_message[full_message_length] = 0;
                    uint16_t start = 0;
                    // Break the packet up into messages (delimited by Z.)
                    for(uint16_t i=0;i<full_message_length;i++) {
                        if(udp_message[i] == 'Z') {
                            udp_message[i] = 0;
                            udp_message_counter++;
                            message_start_pointer = udp_message + start;
                            message_length = i - start;
                            // tell the parse task, time to parse this message into deltas and add to the queue
                            xTaskNotifyGive(alles_parse_handle);
                            // And wait for it to come back
                            ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
                            start = i+1;
                        }
                    }
                }
            }
            // Do a ping every so often
            int64_t sysclock = esp_timer_get_time() / 1000;
            if(sysclock > (last_ping_time+PING_TIME_MS)) {
                ping(sysclock);
            }
        }

        ESP_LOGE(TAG, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }

}



