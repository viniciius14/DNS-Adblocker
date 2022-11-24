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


// void add_char(char to_add, char* word)
// {
//     if(strlen(word) == 0){
//         word = (char*)realloc(word, 1);
//         word[strlen(word)] = '\0';
//     }
//     word = (char*)realloc(word, strlen(word) + 1);
//     word[strlen(word) - 1] = to_add;
// }

void get_clean_address(struct Message_Query *Q_ptr, char* address)
{
    char *a = Q_ptr->question.QNAME;
    int i = 0, num = 0;
    char dot = '.';
    // Transform our 6google3com0 into google.com
    do
    {
        if (num == 0)
        {
            num = a[i] - '0' + 1;
            if(i != 0){
                strncat(address, &dot, 1);
            }
        }
        else{strncat(address, &a[i], 1);}
        i++;num--;
    }
    while(a[i] != '0');

}

void get_ip_address(struct Message_Query *Q_ptr, char *ip_addr)
{
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;    

    char* addr = (char*)malloc(sizeof(char));
    get_clean_address(Q_ptr, addr);
    
    if ((status = getaddrinfo(addr, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return;
    }

    // Will only work for IPv4
    // for(p = res; p != NULL; p = p->ai_next) {
    //     void *addr;

    //     struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
    //     addr = &(ipv4->sin_addr);

    //     // convert the IP to a string and print it:
    //     inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    // }
    {// Will return info on the first element of the linked only
        void *addr;
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        addr = &(ipv4->sin_addr);
        // convert the IP to a string and print it:
        inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
        strcpy(ip_addr, ipstr);
        return;
    } 
}

// This funcion will create our dns packet for replys
void create_packet(struct Message_Query *Q_ptr, struct Message_Response *R_ptr)
{
    char *ip_addr;
    get_ip_address(Q_ptr, ip_addr);

    struct Header new_header = {
        .ID         = Q_ptr->header.ID,   // Varies everytime
        .QR         = 0x01,     // It's a response
        .OPCODE     = 0x00,     // Standard query (0)
        .AA         = 0x00,     // Server is not an authority for domain
        .TC         = 0x00,     // Message is not truncated
        .RD         = 0x01,     // Do query recursively
        .RA         = 0x01,     // Server can do recursive queries
        .Z          = 0x00,
                    //0x00      // Answer authenticated: Answer/authority portion was not authenticated by the server
                    //0x00      // Non-authenticated data: Unacceptable
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
        .TYPE       = 0x01,     // Type A
        .CLASS      = 0x01,     // Class: IN (0x0001)
        .TTL        = 0x3c,     // Time to live (1 minute)
        .RDLENGTH   = 0x04,     // length of RDATA
        .RDATA      = ip_addr   //{0xac, 0xd9, 0xa8, 0xae} // 4 octet ARPA Internet address
    };
    struct Resource new_additional = {
        .NAME       = 0x00,     // <Root>
        .TYPE       = 0x29,     // OPT 41
        .CLASS      = 0x0200,   // UDP payload size
                    //0x00      // "Higher bits in extended RCODE: 0x00"
        //EDNS0 version: 0
        .TTL        = 0x3c,     // Time to live (1 minute)
        .RDLENGTH   = 0x00,     // Length of RDATA
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
            .QNAME  = "6google3com0",
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

	printf("listener: got packet from %s\n", 
        inet_ntop(their_addr.ss_family, 
        get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains: ");

    for(int i = 0 ; i != numbytes ; i++){
        printf("%c",buf[i]);
    }
    printf("\n");

    printf("Replying.\n");

    struct Message_Response *response = (struct Message_Response*)malloc(sizeof(struct Message_Response));
    create_packet(&SGQ, response);

    printf("Made it here");

    if ((numbytes = sendto(sockfd, response, sizeof(struct Message_Response), 0,
            p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    printf("server: response of %d bytes\n", numbytes);







	close(sockfd);

	return 0;
}