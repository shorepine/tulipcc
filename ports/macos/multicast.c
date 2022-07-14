// multicast_desktop.c
#include "alles.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include <ifaddrs.h>
#include <netdb.h>

extern void deserialize_event(char * message, uint16_t length);

int sock= -1;
uint8_t ipv4_quartet;
//extern uint8_t quartet_offset;
char udp_message[MAX_RECEIVE_LEN];
extern char *message_start_pointer;
extern char *alles_local_ip;
extern int16_t message_length;
uint32_t udp_message_counter = 0;
int64_t last_ping_time = PING_TIME_MS; // do the first ping at 10s in to wait for other synths to announce themselves


// Gets the first non-localhost IP address if the user did not specify one on the commandline.
int get_first_ip_address(char *host) {
	struct ifaddrs *ifaddr, *ifa;
    int s;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
        if (ifa->ifa_addr == NULL)
            continue;
        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if(ifa->ifa_addr->sa_family==AF_INET) {
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            // Skip localhost 
            if(strncmp(ifa->ifa_name, "lo", 2) == 0) continue;
            // Return the first one
            return 0;
        }
    }

    freeifaddrs(ifaddr);
    return 1;
}

int socket_add_ipv4_multicast_group() {
	struct ip_mreq imreq;
    struct in_addr iaddr;
    int err = 0;

    // Configure multicast address to listen to
    inet_pton(AF_INET, MULTICAST_IPV4_ADDR, &(imreq.imr_multiaddr.s_addr));
    inet_pton(AF_INET, alles_local_ip, &(iaddr.s_addr));

    // Get the ipv4 "quartet" (last # of 4) and add the offset to it if one
    ipv4_quartet = ((iaddr.s_addr & 0xFF000000) >> 24);

    // Assign the IPv4 multicast source interface, via its IP
    err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
                     sizeof(struct in_addr));
    if (err < 0) {
    	fprintf(stderr, "Failed to set IP_MULTICAST_IF. Error %d\n", errno);
        goto err;
    }

    imreq.imr_interface = iaddr;
    err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                         &imreq, sizeof(struct ip_mreq));
    if (err < 0) {
        fprintf(stderr, "Failed to set IP_ADD_MEMBERSHIP. Error %d\n", errno);
        goto err;
    }

 err:
    return err;


}

void create_multicast_ipv4_socket(void) {
    struct sockaddr_in saddr = { 0 };
    sock = -1;
    int err = 0;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        fprintf(stderr, "Failed to create socket. Error %d\n", errno);
        exit(1);
    }

    int yes = 1;
    err = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int));
    if(err<0) fprintf(stderr, "Can't set reuseport %d\n",errno);
    err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if(err<0) fprintf(stderr, "Can't set reuseaddr %d\n",errno);

    // Bind the socket to any address
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(UDP_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    err = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (err < 0) {
    	fprintf(stderr, "failed to bind socket - %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // Assign multicast TTL (set separately from normal interface TTL)
    uint8_t ttl = MULTICAST_TTL;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(uint8_t));
    if (err < 0) {
    	fprintf(stderr, "failed to set IP_MULTICAST_TTL  %d\n", errno);
        exit(EXIT_FAILURE);
    }

    uint8_t loopback_val = 1;
    err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
                     &loopback_val, sizeof(uint8_t));
    if (err < 0) {
    	fprintf(stderr, "failed to set IP_MULTICAST_LOOP  %d\n", errno);
        exit(EXIT_FAILURE);
    }

    err = socket_add_ipv4_multicast_group();
    if(err) exit(EXIT_FAILURE);

    printf("Multicast IF is %s. Client tag (not ID) is %d. Listening on %s:%d\n", alles_local_ip, ipv4_quartet, MULTICAST_IPV4_ADDR, UDP_PORT);
}


void mcast_send(char * message, uint16_t len) {
    struct addrinfo hints = {
        .ai_flags = AI_PASSIVE,
        .ai_socktype = SOCK_DGRAM,
    };
    struct addrinfo *res;
    hints.ai_family = AF_INET; // For an IPv4 socket
    int err = getaddrinfo(MULTICAST_IPV4_ADDR, NULL, &hints, &res);
    if (err < 0) {
        fprintf(stderr, "getaddrinfo() failed for IPV4 destination address. error: %d", err);
    }
    if (res == 0) {
        fprintf(stderr, "getaddrinfo() did not return any addresses");
    }
    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(UDP_PORT);
    err = sendto(sock, message, len, 0, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    if (err < 0) {
        fprintf(stderr, "IPV4 sendto failed. errno: %d", errno);
    }
}



// called from pthread
void *mcast_listen_task(void *vargp) {
    struct timeval tv = {
        .tv_sec =  1,
        .tv_usec = 0,
    };

    int16_t full_message_length;
    while (1) {
        // set destination multicast addresses for sending from these sockets
        //struct sockaddr_in sdestv4 = {
        //    .sin_family = AF_INET,
         //   .sin_port = htons(UDP_PORT),
        //};
        // We know this inet_aton will pass because we did it above already
        //inet_pton(AF_INET, MULTICAST_IPV4_ADDR, &(sdestv4.sin_addr.s_addr));

        int err = 1;
        while (err > 0) { 
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(sock, &rfds);

            int s = select(sock + 1, &rfds, NULL, NULL, &tv);
            if (s < 0) {
                fprintf(stderr, "Select failed: errno %d\n", errno);
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
                        fprintf(stderr, "multicast recvfrom failed: errno %d\n", errno);
                        err = -1;
                        break;
                    }
                    udp_message[full_message_length] = 0;
                    uint16_t start = 0;
                    // Break the packet up into messages (delimited by \n.)
                    for(uint16_t i=0;i<full_message_length;i++) {
                        if(udp_message[i] == 'Z') {
                            udp_message[i] = 0;
                            udp_message_counter++;
                            message_start_pointer = udp_message + start;
                            message_length = i - start;
                            parse_task();
                            start = i+1;
                        }
                    }
                }
            } 
            // Do a ping every so often
            int64_t sysclock = get_sysclock();
            if(sysclock > (last_ping_time+PING_TIME_MS)) {
                ping(sysclock);
            }
            usleep(THREAD_USLEEP);
        }

        fprintf(stderr, "Shutting down socket and restarting...\n");
        shutdown(sock, 0);
        close(sock);
    }
}

