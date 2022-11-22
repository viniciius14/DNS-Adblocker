#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../Headers/dns.h"



#define PORT "53"















// // This funcion will create our dns packet for replys
// struct Answer* create_packet(struct Query* ptr)
// {

//     struct Answer ans = {
//         .ID = ptr->ID,
//         .flags = {
//             .RESPONSE = 1,
//             .OPCODE = 0,
//             .
//         }

//     }
   





//     return 
// }




int main(void){

    struct Message_Query SGQ = {
        .header = {
            .ID         = 0x700b,   // Varies everytime
            .QR         = 0x00,     // It's a query
            .Opcode     = 0x00,     // Standard query (0)
            .AA         = 0x00,     // Only for responses
            .TC         = 0x00,     // Message is not truncated
            .RD         = 0x01,     // Do query recursively
            .RA         = 0x00,     // Only for responses
            .Z          = 0x00,
                        //0x01      // AD bit: Set (not sure what this is)
            .RCODE      = 0x00,     // Only for responses
            .QDCOUNT    = 0x01,     // Questions: 1
            .ANCOUNT    = 0x00,     // Resource records in answer section
            .NSCOUNT    = 0x00,     // Server resource records in the authority records section
            .ARCOUNT    = 0x01      // Resource records in the additional records section
        },
        .question = {
            .QNAME  = "googlecom",
            .QTYPE  = 0x01,         // Type A
            .QCLASS = 0x01          // Class: IN (0x0001)
        },
        .additional = {
            .NAME       = 0x00,     // <Root>
            .TYPE       = 0x29,     // OPT 41
            .CLASS      = 0x1000,   // UDP payload size
            //0x00                  // "Higher bits in extended RCODE: 0x00"
            //EDNS0 version: 0
            .TTL        = 0x00,     // Not specified
            .RDLENGTH   = 0x0c,     // Length of RDATA
            .RDATA      = 0x00      // Not specified
        }
    };
    

    

}











// int main(int argc, char **argv)
// {
//     int sockfd, rv;
//     char *hostname, ip[100];

//     struct addrinfo hints, *response, *p;
//     struct sockaddr_in *ip_access;
    




    
//     if (argc != 2)
//     {
//         printf("Usage: ./program address");
//         exit(1);
//     }

//     strcpy(hostname, argv[1]);

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_UNSPEC;
//     hints.ai_socktype = SOCK_DGRAM; // UDP socket

//     if ((rv = getaddrinfo(hostname, "domain", &hints, &response)) != 0)
//     {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//         return 1;
//     }

//     for (p = response; p != NULL; p = p->ai_next)
//     {
//         ip_access = (struct sockaddr_in *)p->ai_addr;

//         printf("IP address is %s: \n", inet_ntoa(ip_access->sin_addr));
//     }

//     freeaddrinfo(response);
// }