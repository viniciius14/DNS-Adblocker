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

#include "../Headers/dns_utils.h"


#define PORT "4950"  // -> to change too 53
#define MAXBUFLEN 100


// this funcion will create our dns packet for replys
void create_packet(struct Message_Query *Q_ptr, struct Message_Response *R_ptr)
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

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP


    struct Message_Response *test = (struct Message_Response*)malloc(sizeof(struct Message_Response));
    // standard google query
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



	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains: ", buf);

    for(int i = 0 ; i != numbytes ; i++){
        printf("%c",buf[i]);
    }
    printf("\n");

	close(sockfd);

	return 0;
}