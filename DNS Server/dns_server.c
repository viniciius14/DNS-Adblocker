#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../Headers/dns_utils.h"



#define PORT "53"









// This funcion will create our dns packet for replys
void create_packet(struct Message_Query* Q_ptr, struct Message_Response* R_ptr)
{

    unsigned char new_rdata[] = {0xac, 0xd9, 0xa8, 0xae};

    struct Header new_header = {
        .ID         = Q_ptr->header.ID,   // Varies everytime
        .QR         = 0x01,     // It's a response
        .OPCODE     = 0x00,     // Standard query (0)
        .AA         = 0x00,     // Server is not an authority for domain
        .TC         = 0x00,     // Message is not truncated
        .RD         = 0x01,     // Do query recursively
        .RA         = 0x01,     // Server can do recursive queries
        .Z          = 0x00,
        //0x00                  // Answer authenticated: Answer/authority portion was not authenticated by the server
        //0x00                  // Non-authenticated data: Unacceptable
        .RCODE      = 0x00,     // No error
        .QDCOUNT    = 0x01,     // Questions: 1
        .ANCOUNT    = 0x01,     // Resource records in answer section
        .NSCOUNT    = 0x00,     // Server resource records in the authority records section
        .ARCOUNT    = 0x01      // Resource records in the additional records section
    };
    struct Question new_question = {
        .QNAME      = Q_ptr->question.QNAME,
        .QTYPE      = 0x01,     // Type A
        .QCLASS     = 0x01      // Class: IN (0x0001)
    };
    struct Resource new_answer = {
        .NAME       = Q_ptr->question.QNAME,
        .TYPE       = 0x01,         // Type A
        .CLASS      = 0x01,         // Class: IN (0x0001)
        .TTL        = 0x54,         // 84 seconds
        .RDLENGTH   = 0x04,         // length of RDATA
        .RDATA      = new_rdata  // 4 octet ARPA Internet address
    };
    struct Resource new_additional = {
        .NAME       = 0x00,     // <Root>
        .TYPE       = 0x29,     // OPT 41
        .CLASS      = 0x0200,   // UDP payload size
        //0x00                  // "Higher bits in extended RCODE: 0x00"
        //EDNS0 version: 0
        .TTL        = 0x00,     // Not specified
        .RDLENGTH   = 0x0c,     // Length of RDATA
        .RDATA      = 0x00      // Not specified
    };

    R_ptr->header = new_header;
    R_ptr->question = new_question; 
    R_ptr->answer = new_answer; 
    R_ptr->additional = new_additional;

    //printf("%x\n%s\n",R_ptr->header.ID, R_ptr->question.QNAME);

}




int main(void){

    struct Message_Response* test = (struct Message_Response*)malloc(sizeof(struct Message_Response));

    struct Message_Query SGQ = {
        .header = {
            .ID         = 0x700b,   // Varies everytime
            .QR         = 0x00,     // It's a query
            .OPCODE     = 0x00,     // Standard query (0)
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


    struct Message_Query* ptr = &SGQ;




    create_packet(ptr, test);

    

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