#include "../Headers/dns_utils.h"
#include "../Headers/hosts.h"

int main(void){

    //struct Message_Query *buf = (struct Message_Query*)malloc(sizeof(struct Message_Query));

    unsigned char buf[500], *hostname = (char *)malloc(1);
    int size = 0, i = 0, y = 0;


    while(1)
    {
        // if((size = await_receive(buf,4950)) == 0)
        // {
        //     exit(1);
        // }


        // for(int i = 0 ; i != size ; i++){
        //     printf("%x",buf[i]);
        // }
        // printf("\n");

        // now we decode
        // find out whats been asked

        unsigned char buf[] = {
            0x4e, 0x93, 0x01, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x06, 0x67, 0x6f,
            0x6f, 0x67, 0x6c, 0x65, 0x03,
            0x63, 0x6f, 0x6d, 0x00, 0x00,
            0x01, 0x00, 0x01
        };

        struct Header *header = (struct Header*)calloc(1,sizeof(struct Header));
        struct Question *quest = (struct Question*)calloc(1,sizeof(struct Question));
                
        memcpy(&(header->ID),       buf + i, 2); i += 2;
        memcpy(&(header->FLAGS),    buf + i, 2); i += 2;
        memcpy(&(header->QDCOUNT),  buf + i, 2); i += 2;
        memcpy(&(header->ANCOUNT),  buf + i, 2); i += 2;
        memcpy(&(header->NSCOUNT),  buf + i, 2); i += 2;
        memcpy(&(header->ARCOUNT),  buf + i, 2); i += 2;


        //get hostname
        do{
            if((int)buf[i] >= 58 ){
                hostname = realloc(hostname, sizeof(hostname) + 1);
                hostname[y] = buf[i];
                y++;
            }
            else{
                if(y == 0){
                    i++;
                    continue;
                }
                hostname = realloc(hostname, sizeof(hostname) + 1);
                hostname[y] = '.';
                y++;
            }
            
            i++;
        }while(buf[i] !=  0x00);
        i++;

        memcpy(&(quest->QTYPE),   buf + i, 2); i += 2;
        memcpy(&(quest->QCLASS),  buf + i, 2); i += 2;


        swap_16_bit(&(header->ID));
        swap_16_bit(&(header->FLAGS));
        swap_16_bit(&(header->QDCOUNT));
        swap_16_bit(&(header->ANCOUNT));
        swap_16_bit(&(header->NSCOUNT));
        swap_16_bit(&(header->ARCOUNT));

        swap_16_bit(&(quest->QTYPE));
        swap_16_bit(&(quest->QCLASS));


        struct Header_Flags flags = decode_header_flags(header->FLAGS);

        
        printf("Transaction ID: %x\n",       header->ID);

        printf("Response: %d\n",             flags.QR);
        printf("Type of query: %d\n",        flags.OPCODE);
        printf("Authoritative answer: %d\n", flags.AA);
        printf("Truncation: %d\n",           flags.TC);
        printf("Recursion desired: %d\n",    flags.RD);
        printf("Recursion available: %d\n",  flags.RA);
        printf("Response code: %d\n",        flags.RCODE);

        printf("Querys: %d\n",               header->QDCOUNT);
        printf("Answer RRs: %d\n",           header->ANCOUNT);
        printf("Authority RRs: %d\n",        header->NSCOUNT);
        printf("Additional RRs: %d\n",       header->ARCOUNT);

        printf("Hostname: %s\n",             hostname);
        printf("Type: %d\n",                 quest->QTYPE);
        printf("Class: %d\n",                quest->QCLASS);


        //search file for hostname
        char *host = find_host(hostname);
        
    


        if(host){
            //return IP OF HTTP SERVER
        }
        else{
            // ask google / await response / send the results to ourselves
            if(dns_send(buf, sizeof(buf), 53, "8.8.8.8") == 0)
            {
                int size = 0;
                unsigned char buf[500] = {0};
                if((size = await_receive(buf, 53)) != 0){
                    //send to original adress
                    dns_send(buf, size, 53, "::1");
                }
            }
        }
        memset(buf, 0, sizeof(buf));

        free(hostname);
        char *hostname = (char *)malloc(1);

        size = 0, i = 0, y = 0;
    }
}