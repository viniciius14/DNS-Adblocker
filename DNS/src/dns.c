#include <stdio.h>


#define PORT "53"

struct Header{
    __uint16_t ID;
    __uint16_t QR:1;
    __uint16_t Opcode:4;
    __uint16_t AA:1;
    __uint16_t TC:1;
    __uint16_t RD:1;
    __uint16_t RA:1;
    __uint16_t Z:3;
    __uint16_t RCODE:4;
    __uint16_t QDCOUNT;
    __uint16_t ANCOUNT;
    __uint16_t NSCOUNT;
    __uint16_t ARCOUNT;
};

struct Question{
    char*  QNAME;
    __uint16_t QTYPE;
    __uint16_t QCLASS;
};

struct Resource{
    char*  NAME;
    __uint16_t TYPE;
    __uint16_t CLASS;
    __uint32_t TTL;
    __uint16_t RDLENGTH;
    char* RDATA;
};

//"The answer, authority, and additional sections all share the same format"
struct Message{
    struct Header header;
    struct Question question;
    struct Resource answer;
    struct Resource authority;
    struct Resource additional;
};


int main(int argc, char** argv){






    return 0;
}