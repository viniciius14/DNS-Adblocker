#include "../Headers/dns_utils.h"
#include "../Headers/hosts.h"

int main(void){

    unsigned char buf[500], *hostname = (char *)malloc(1);
    int size = 0, i = 0, y = 0;


    while(1)
    {
        if((size = await_receive(buf, "53")) == 0)
        {
            exit(1);
        }
        // decode & find out whats been asked

        // dns query for google.com
        // unsigned char buf[] = {
        //     0x4e, 0x93, 0x01, 0x00, 0x00,
        //     0x01, 0x00, 0x00, 0x00, 0x00,
        //     0x00, 0x00, 0x06, 0x67, 0x6f,
        //     0x6f, 0x67, 0x6c, 0x65, 0x03,
        //     0x63, 0x6f, 0x6d, 0x00, 0x00,
        //     0x01, 0x00, 0x01
        // };

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


        // search file for hostname
        char *host = find_host(hostname);    

        break;
        if(host){
            // return IP OF HTTP SERVER -- since its in current machine
            // ip == 127.0.0.1

            struct Header r_header;
            struct Header_Flags r_flag;
            struct Question r_quest;
            struct Resource r_answer;

            r_header.ID = header->ID;

            r_flag.QR     = 1;
            r_flag.OPCODE = 0;
            r_flag.AA     = 0;
            r_flag.TC     = 0;
            r_flag.RD     = 1;
            r_flag.RA     = 0;
            r_flag.Z      = 0;
            r_flag.RCODE  = 0;

            r_header.FLAGS   = encode_header_flags(r_flag);
            r_header.QDCOUNT = 1;
            r_header.ANCOUNT = 1;
            r_header.NSCOUNT = 0;
            r_header.ARCOUNT = 0;

            encode_hostname(r_quest.QNAME, hostname);
            r_quest.QTYPE  = quest->QTYPE;
            r_quest.QCLASS = quest->QCLASS;

            encode_hostname(r_answer.NAME, hostname);
            r_answer.TYPE       = quest->QTYPE;
            r_answer.CLASS      = quest->QCLASS;
            r_answer.TTL        = 0;
            r_answer.RDLENGTH   = 4;  // size in octets of ip
            r_answer.RDATA      = "127.0.0.1";

            unsigned char *reply = (unsigned char *)calloc(1,
                                sizeof(struct Header) + 
                                sizeof(struct Question) + 
                                sizeof(struct Resource));

            int offset = 0;
            reply   = memcpy((reply + offset), &r_header, sizeof(struct Header)); 
            offset += sizeof(struct Header);
            reply   = memcpy((reply + offset), &r_quest, sizeof(struct Question));
            offset += sizeof(struct Question);
            reply   = memcpy((reply + offset), &r_answer, sizeof(struct Resource));

            if(dns_send(reply, sizeof(reply), "53", "127.0.0.1") != 0)
            {
                exit(1);
            }
            
        }
        else{
            // ask google / await response / send the results to ourselves
            if(dns_send(buf, sizeof(buf), "53", "8.8.8.8") == 0)
            {
                int size = 0;
                unsigned char buf[500];
                memset(buf, 0, sizeof(buf));

                if((size = await_receive(buf, "53")) != 0){
                    //send to original address
                    dns_send(buf, size, "53", "::1");
                }
            }
        }
        memset(buf, 0, sizeof(buf));

        free(hostname);
        char *hostname = (char *)malloc(1);

        size = 0, i = 0, y = 0;
    }
}
