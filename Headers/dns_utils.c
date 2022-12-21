#include "dns_utils.h"

#define PORT "4950" //change to 53
#define MAXBUFLEN 100


void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int await_receive(unsigned char *buf)
{
    int sockfd, rv, numbytes;
	char s[INET6_ADDRSTRLEN];    

	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;

	socklen_t addr_len;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo("::1", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("waiting to recvfrom...\n");

    if ((numbytes = recvfrom(sockfd, &buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
	printf("packet is %d bytes long\n", numbytes);

    // printf("packet contents in a hex stream ");
    // for(int i = 0 ; i != numbytes ; i++){
    //     printf("%x",buf[i]);
    // }
    // printf("\n");
	return 0;
}

int dns_send(unsigned char *buf, size_t size)
{
    int sockfd, rv, numbytes;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo("::1", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "failed to create socket\n");
		return 2;
	}

	if ((numbytes = sendto(sockfd, &buf, size, 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("sent %d bytes to localhost\n", numbytes);
	close(sockfd);
	
	return 0;
}





int encode_hostname(char *dest, char *hostname) {
    // www.google.com
    // 3www
    // 6google
    // 3com
    // 0
    char *token = strtok(hostname, ".");

    int len = 0;
    int offset = 0;
    while (token != NULL) {
        len = strlen(token);
        dest[offset] = len;
        offset++;
        strncpy(dest+offset, token, len);
        offset += len;
        token = strtok(NULL, ".");
    }
    dest[offset] = 0;
    return offset+1;
}


int decode_hostname(char *src, char **hostname) {
    int len = 0;
    while (src[len] != 0) {
        if (len > 0) (*hostname)[len-1] = '.';
        *hostname = realloc(*hostname, src[len]+1);
        memcpy(*hostname+len, src+len+1, src[len]);
        len+=src[len]+1;
    }
    (*hostname)[len]='\0';
    return len+1;
}


// // flags to uint16_t encoder
// uint16_t dns_header_flags_encode(dns_header_flags_t flags) {
//     return (uint16_t) ((flags.qr << 15) | (flags.opcode << 11) | (flags.aa << 10) | (flags.tc << 9) | (flags.rd << 8) |
//                        (flags.ra << 7) | (flags.z << 4) | flags.rcode);
// }

// // uint16_t to flags decoder
// dns_header_flags_t dns_header_flags_decode(uint16_t value) {
//     dns_header_flags_t flags;
//     flags.qr = (value >> 15)&1;
//     flags.opcode = (value >> 11)&15;
//     flags.aa = (value >> 10)&1;
//     flags.tc = (value >> 9)&1;
//     flags.rd = (value >> 8)&1;
//     flags.ra = (value >> 7)&1;
//     flags.z = (value >> 4)&7;
//     flags.rcode = value&15;
//     return flags;
// }