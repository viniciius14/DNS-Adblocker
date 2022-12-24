#ifndef DNS_H
#define DNS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../Headers/dns_utils.c"


struct Header
{
    __uint16_t ID;
    __uint16_t FLAGS;
    __uint16_t QDCOUNT;
    __uint16_t ANCOUNT;
    __uint16_t NSCOUNT;
    __uint16_t ARCOUNT;
};

struct Question
{
    char *QNAME;
    __uint16_t QTYPE;
    __uint16_t QCLASS;
};

struct Resource
{
    char *NAME;
    __uint16_t TYPE;
    __uint16_t CLASS;
    __uint32_t TTL;
    __uint16_t RDLENGTH;
    char *RDATA;
};

struct Message_Query
{
    struct Header header;
    struct Question question;
    struct Resource additional;
};

struct Message_Response
{
    struct Header header;
    struct Question question;
    struct Resource answer;
    struct Resource additional;
};



void *get_in_addr(struct sockaddr *sa);
void swap_16_bit(__uint16_t *num);

int await_receive(unsigned char *buf, int port);
int dns_send(unsigned char *buf, size_t size, const int port, const char *address);

int encode_hostname(char *dest, char *hostname);
__uint16_t encode_header_flags(struct Header_Flags flags);


#endif
