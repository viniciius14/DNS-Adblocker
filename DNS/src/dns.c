#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#define PORT "53"

struct __attribute__((__packed__)) Header
{
    __uint16_t ID;      
    __uint16_t QR : 1;
    __uint16_t Opcode : 4;
    __uint16_t AA : 1;
    __uint16_t TC : 1;
    __uint16_t RD : 1;
    __uint16_t RA : 1;
    __uint16_t Z : 3;
    __uint16_t RCODE : 4;
    __uint16_t QDCOUNT;
    __uint16_t ANCOUNT;
    __uint16_t NSCOUNT;
    __uint16_t ARCOUNT;
};

struct __attribute__((__packed__)) Question
{
    char *QNAME;
    __uint16_t QTYPE;
    __uint16_t QCLASS;
};

struct __attribute__((__packed__)) Resource
{
    char *NAME;
    __uint16_t TYPE;
    __uint16_t CLASS;
    __uint32_t TTL;
    __uint16_t RDLENGTH;
    char *RDATA;
};

// "The answer, authority, and additional sections all share the same format"
struct __attribute__((__packed__)) Message
{
    struct Header header;
    struct Question question;
    struct Resource answer;
    struct Resource authority;
    struct Resource additional;
};

// Resolver:
// timestamp of when request began uses the absolute time format previously discussed for RR storage
// parameter to limit the amount of work wich will be performed for this request

// int main(int argc, char** argv){

//     /*
//     infinite loop
//     take in calls
//     check if received a calida dns package
//     if not reply error
//     else analyse package
//     check if domain name is in our hosts list
//     if it is return ip of our http server
//     else call a function that calls an actual dns server
//     and return whatever that returns
//     */

//     return 0;
// }

int main(int argc, char **argv)
{
    int sockfd, rv;
    char *hostname, ip[100];

    struct addrinfo hints, *response, *p;
    struct sockaddr_in *ip_access;

    // Since objects can't be set to null they're gonna be set to 0
    
    // Standard google dns query, hardcoded
    struct Message SGQ = {
        .header = {
            .ID         = 0x700b,   // Varies everytime
            .QR         = 0x00,     // It's a query
            .Opcode     = 0x00,     // Standard query (0)
            .AA         = 0x00,     // Only for responses
            .TC         = 0x00,     // Message is not truncated
            .RD         = 0x01,     // Do query recursively
            .RA         = 0x00,     // Only for responses
            .Z          = 0x00,
            //0x01                  // AD bit: Set (not sure what this is)
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
        .answer = {
            0
        },
        .authority = {
            0
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

    // Standard google dns answer, hardcoded
    struct Message SGA = {
        .header = {
            .ID         = 0x700b,   // varies everytime
            .QR         = 0x01,     // It's a response
            .Opcode     = 0x00,     // Standard query (0)
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
        },
        .question = {
            .QNAME      = "googlecom",
            .QTYPE      = 0x01,     // Type A
            .QCLASS     = 0x01      // Class: IN (0x0001)
        },
        .answer = {
            .NAME       = "googlecom",
            .TYPE       = 0x01,         // Type A
            .CLASS      = 0x01,         // Class: IN (0x0001)
            .TTL        = 0x54,         // 84 seconds
            .RDLENGTH   = 0x04,         // length of RDATA
            .RDATA      = 0xacd9a8ae    // 4 octet ARPA Internet address
        },
        .authority = {
            0
        },
        .additional = {
            .NAME       = 0x00,     // <Root>
            .TYPE       = 0x29,     // OPT 41
            .CLASS      = 0x0200,   // UDP payload size
            //0x00                  // "Higher bits in extended RCODE: 0x00"
            //EDNS0 version: 0
            .TTL        = 0x00,     // Mot specified
            .RDLENGTH   = 0x0c,     // Length of RDATA
            .RDATA      = 0x00      // Mot specified
        }
    };
    
    /*
    printf("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
        SGQ.header.ID,
        SGQ.header.QR,
        SGQ.header.Opcode,
        SGQ.header.AA,
        SGQ.header.TC,
        SGQ.header.RD,
        SGQ.header.RA,
        SGQ.header.Z,
        SGQ.header.RCODE,
        SGQ.question.QNAME,
        SGQ.question.QTYPE,
        SGQ.question.QCLASS,
        SGQ.additional.TYPE,
        SGQ.additional.CLASS,
        SGQ.additional.TTL,
        SGQ.additional.RDLENGTH,
        SGQ.additional.RDATA);
    */
    
    
    
    if (argc != 2)
    {
        printf("Usage: ./program address");
        exit(1);
    }

    strcpy(hostname, argv[1]);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    if ((rv = getaddrinfo(hostname, "domain", &hints, &response)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = response; p != NULL; p = p->ai_next)
    {
        ip_access = (struct sockaddr_in *)p->ai_addr;

        printf("IP address is %s: \n", inet_ntoa(ip_access->sin_addr));
    }

    freeaddrinfo(response);
}