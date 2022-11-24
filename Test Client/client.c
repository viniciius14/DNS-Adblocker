/*
** talker.c -- a datagram "client" demo
*/

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

#define SERVERPORT "4950"	// the port users will be connecting to
#define MAXBUFLEN 100

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	unsigned char packet[] = {
		0x5f, 0xa6, 0x01, 0x20, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x01, 0x06, 0x67, 0x6f, 
		0x6f, 0x67, 0x6c, 0x65, 0x03, 
		0x63, 0x6f, 0x6d, 0x00, 0x00,
		0x01, 0x00, 0x01, 0x00, 0x00,
		0x29, 0x10, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x0c, 0x00, 
		0x0a, 0x00, 0x08, 0x45, 0x40,
		0xa5, 0x9a, 0x21, 0x27, 0x2f, 0x48
	};


	// if (argc != 3) {
	// 	fprintf(stderr,"usage: talker hostname \n");
	// 	exit(1);
	// }

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	if ((numbytes = sendto(sockfd, packet, sizeof(packet), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	//freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);



	socklen_t addr_len;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	char s[INET6_ADDRSTRLEN];

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}	

	printf("client: got packet from %s\n", 
        inet_ntop(their_addr.ss_family, 
        get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    
	printf("client: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("client: packet contains: ");

    for(int i = 0 ; i != numbytes ; i++){
        printf("%c",buf[i]);
    }
    printf("\n");



	freeaddrinfo(servinfo);

	close(sockfd);

	return 0;
}