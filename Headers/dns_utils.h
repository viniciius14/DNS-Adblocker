#ifndef DNS_H
#define DNS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "dns_utils.c"



struct __attribute__((__packed__)) Header
{
    __uint16_t ID;      
    __uint16_t QR : 1;
    __uint16_t OPCODE : 4;
    __uint16_t AA : 1;
    __uint16_t TC : 1;
    __uint16_t RD : 1;
    __uint16_t RA : 1;
    __uint16_t Z  : 3;
    __uint16_t RCODE  : 4;
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

struct __attribute__((__packed__)) Message_Query
{
    struct Header header;
    struct Question question;
    struct Resource additional;
};

struct __attribute__((__packed__)) Message_Response
{
    struct Header header;
    struct Question question;
    struct Resource answer;
    struct Resource additional;
};

void *get_in_addr(struct sockaddr *sa);

int await_receive(unsigned char *buf);

int dns_send(unsigned char *buf, size_t size);


int encode_hostname(char *dest, char *hostname);
int decode_hostname(char *src, char **hostname);

// uint16_t dns_header_flags_encode(dns_header_flags_t flags);

// dns_header_flags_t dns_header_flags_decode(uint16_t value);


#endif












// void *respond(char *reply) 
// {
//     int sockfd, rv, numbytes;
// 	char buf[MAXBUFLEN];
// 	char s[INET6_ADDRSTRLEN]; 

// 	struct addrinfo hints, *servinfo, *p;
// 	struct sockaddr_storage their_addr;   

//     socklen_t addr_len;

//     memset(&hints, 0, sizeof hints);
// 	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
// 	hints.ai_socktype = SOCK_DGRAM;

// 	if ((rv = getaddrinfo("::1", PORT, &hints, &servinfo)) != 0) {
// 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
// 		return NULL;
// 	}

// 	// loop through all the results and make a socket
// 	for(p = servinfo; p != NULL; p = p->ai_next) {
// 		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
// 			perror("socket");
// 			continue;
// 		}

// 		break;
// 	}

// 	if (p == NULL) {
// 		fprintf(stderr, "failed to create socket\n");
// 		return NULL;
// 	}

// 	if ((numbytes = sendto(sockfd, reply, sizeof(reply), 0, p->ai_addr, p->ai_addrlen)) == -1) {
// 		perror("sendto");
// 		exit(1);
// 	}

// 	//freeaddrinfo(servinfo);

// 	printf("sent %d bytes to localhost\n", numbytes);

// }




// // "The answer, authority, and additional sections all share the same format"
// struct __attribute__((__packed__)) Message
// {
//     struct Header header;
//     struct Question question;
//     struct Resource answer;
//     struct Resource authority;
//     struct Resource additional;
// };





// All in accordance with wireshark
// Same for response and query
// struct __attribute__((__packed__)) Queries
// {
//     unsigned char* NAME; 
//     __uint16_t TYPE;
//     __uint16_t CLASS;
// };

// // Query 
// struct __attribute__((__packed__)) Q_Flags
// {
//     __uint16_t RESPONSE:1;
//     __uint16_t OPCODE:4;
//     __uint16_t TRUNCATED:1;
//     __uint16_t RECURSION:1;
//     __uint16_t Z:1;
//     __uint16_t AD:1;
//     __uint16_t NON_AUTH:1;
// };

// struct __attribute__((__packed__)) Q_Additional_Records
// {
//     __uint16_t ADD_NAME:8;
//     __uint16_t OPT;
//     __uint16_t UDP_PAYLOAD;
//     __uint16_t RCODE:8;
//     __uint16_t EBNS0:8;
//     __uint16_t Z;
//     __uint16_t LENGTH;
// };

// struct __attribute__((__packed__)) Q_Option_Cookie
// {
//     __uint16_t OPT_CODE;
//     __uint16_t OPT_LENGTH;
//     unsigned long OPT_DATA;
// };

// struct __attribute__((__packed__)) Query    //final struct to be used in the program
// {
//     __uint16_t ID;
//     struct Q_Flags Flags;
//     __uint16_t QUESTIONS;
//     __uint16_t ANSWER_RRS;
//     __uint16_t AUTHORITY_RR;
//     __uint16_t ADDITIONAL_RR;
//     struct Queries Queries;
//     struct Q_Additional_Records Add_Records;

// };


// // Response
// struct __attribute__((__packed__)) R_Flags
// {
//     __uint16_t RESPONSE:1;          // Set to 1 since it's a response
//     __uint16_t OPCODE:4;
//     __uint16_t AUTHORITATIVE:1;
//     __uint16_t TRUNCATED:1;
//     __uint16_t RECURSION_DESIRED:1;
//     __uint16_t RECURSION_AVAILABLE:1;
//     __uint16_t Z:1;
//     __uint16_t ANSWER_AUTHENTICATED:1;
//     __uint16_t NON_AUTH:1;
//     __uint16_t REPLY_CODE:4;        // 0 if no error
// };

// struct __attribute__((__packed__)) R_Answers
// {
//     unsigned char* NAME;
//     __uint16_t TYPE;
//     __uint16_t CLASS;
//     __uint32_t TTL;
//     __uint16_t DATA_LENGTH;
//     __uint32_t ADDRESS;
// };



// struct __attribute__((__packed__)) Response
// {
//     __uint16_t ID;
//     struct R_Flags Flags;
//     __uint16_t QUESTIONS;
//     __uint16_t ANSWER_RRS;
//     __uint16_t AUTHORITY_RR;
//     __uint16_t ADDITIONAL_RR;
//     struct Queries Queries;
//     struct R_Answers Answers[];  // Will be a list with size ANSWER_RRS 
// };


// struct Response ans = {
//     .ID = ptr->ID,
//     .Flags = {
//         .RESPONSE = 1,
//         .OPCODE = 0,
//         .AUTHORITATIVE = 0,
//         .TRUNCATED = 0,
//         .RECURSION_DESIRED = 1,
//         .RECURSION_AVAILABLE = 1,
//         .Z = 0,
//         .ANSWER_AUTHENTICATED = 0,
//         .NON_AUTH = 0,
//         .REPLY_CODE = 0
//     },
//     .QUESTIONS = 1,
//     .ANSWER_RRS = 1,
//     .AUTHORITY_RR = 0,
//     .ADDITIONAL_RR = 1,
//     .Queries = {
//         .NAME = ptr->Queries.NAME,
//         .TYPE = 1,
//         .CLASS = 1
//     },
//     .Answers = {
//         {

//         }
//     }