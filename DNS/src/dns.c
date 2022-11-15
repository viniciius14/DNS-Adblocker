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

//"The answer, authority, and additional sections all share the same format"
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


    //standard google query, hardcoded

    struct Message hex_SGQ = {
        //header
        {
            0xa6f9,//id, changes every request
            0x00,//QR
            0x00,//OP
            NULL,//AA, not specified in wireshark (only for responses)
            0x00,//TC
            0x01,//RD
            NULL,//RA, not specified in wireshark (only for responses)
            0x00,//Z
            //0x1,//AD bit: Set (not sure what this is)
            NULL,//RCODE, not specified in wireshark (only for responses)
            0x01,//QDCOUNT
            0x00,//ANCOUNT
            0x00,//NSCOUNT
            0x01//ARCOUNT
        },
        //question
        {
            "googlecom",//QNAME
            0x0001,//QTYPE
            0x0001//QCLASS
        },
        NULL,//answer
        NULL,//authority
        //additional
        {
            0x00,//NAME <Root>
            0x0029,//TYPE OPT(41)
            0x1000,//CLASS -> UDP payload size on wireshark
            NULL,//TTL since its a query theres no TTL
            0x0000c,//RDLENGTH
            NULL//RDATA -> Don't see it specified in wireshark
        }
    };

    struct Message dec_SGQ = {
        //header
        {
            42745,//id, changes every request
            0,//QR
            0,//OP
            NULL,//AA, not specified in wireshark (only for responses)
            0,//TC
            1,//RD
            NULL,//RA, not specified in wireshark (only for responses)
            0,//Z
            //0x1,//AD bit: Set (not sure what this is)
            NULL,//RCODE, not specified in wireshark (only for responses)
            1,//QDCOUNT
            0,//ANCOUNT
            0,//NSCOUNT
            1//ARCOUNT
        },
        //question
        {
            "googlecom",//QNAME
            1,//QTYPE
            1//QCLASS
        },
        NULL,//answer
        NULL,//authority
        //additional
        {
            0,//NAME <Root>
            41,//TYPE OPT(41)
            4096,//CLASS -> UDP payload size on wireshark
            NULL,//TTL since its a query theres no TTL
            12,//RDLENGTH
            NULL//RDATA -> Don't see it specified in wireshark
        }
    };


    printf("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
        dec_SGQ.header.ID,
        dec_SGQ.header.QR,
        dec_SGQ.header.Opcode,
        dec_SGQ.header.AA,
        dec_SGQ.header.TC,
        dec_SGQ.header.RD,
        dec_SGQ.header.RA,
        dec_SGQ.header.Z,
        dec_SGQ.header.RCODE,
        dec_SGQ.question.QNAME,
        dec_SGQ.question.QTYPE,
        dec_SGQ.question.QCLASS,
        dec_SGQ.additional.TYPE,
        dec_SGQ.additional.CLASS,
        dec_SGQ.additional.TTL,
        dec_SGQ.additional.RDLENGTH,
        dec_SGQ.additional.RDATA);






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