#include "../Headers/dns_utils.h"

int main(void){

    //struct Message_Query *buf = (struct Message_Query*)malloc(sizeof(struct Message_Query));

    unsigned char buf[100];
    int size = 0;

    while(1)
    {
        // if((size = await_receive(buf)) == 0)
        // {
        //     exit(1);
        // }


        // for(int i = 0 ; i != size ; i++){
        //     printf("%x",buf[i]);
        // }
        // printf("\n");

        //now we decode
        //find whats been asked

        // unsigned char buf[] = {
        //     0x4e, 0x93, 0x01, 0x00, 0x00,
        //     0x01, 0x00, 0x00, 0x00, 0x00,
        //     0x00, 0x00, 0x06, 0x67, 0x6f,
        //     0x6f, 0x00, 0x10, 0x67, 0x6c,
        //     0x65, 0x03, 0x63, 0x6f, 0x6d,
        //     0x00, 0x00, 0x01, 0x00, 0x01
        // };
        unsigned char buf[] = {
            0x4e, 0x93, 0x01, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x06, 0x67, 0x6f,
            0x6f, 0x67, 0x6c, 0x65, 0x03,
            0x63, 0x6f, 0x6d, 0x00, 0x00,
            0x01, 0x00, 0x01
        };


        struct Header *header = (struct Header*)calloc(1,sizeof(struct Header)); //= (struct Header*)(buf);
        struct Question *quest = (struct Question*)calloc(1,sizeof(struct Question)); // = (struct Question*)(buf);
        
        int i = 0;

        memcpy(&(header->ID),       buf + i, 2); i += 2;
        memcpy(&(header->FLAGS),    buf + i, 2); i += 2;
        memcpy(&(header->QDCOUNT),  buf + i, 2); i += 2;
        memcpy(&(header->ANCOUNT),  buf + i, 2); i += 2;
        memcpy(&(header->NSCOUNT),  buf + i, 2); i += 2;
        memcpy(&(header->ARCOUNT),  buf + i, 2); i += 2;

        //unreliable
        while(buf[i] != '\0') { 
        memcpy(&(quest->QNAME),     buf + i, 1); i += 1;
        }
        memcpy(&(quest->QTYPE),     buf + i, 2); i += 2;
        memcpy(&(quest->QCLASS),    buf + i, 2); i += 2;


        unsigned char a[] = "aa"; 
        for(int i = 0 ; i != 10 ; i++){
            printf("%c",quest->QNAME[i]);
        }

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

        printf("Questions: %d\n",            header->QDCOUNT);
        printf("Answer RRs: %d\n",           header->ANCOUNT);
        printf("Authority RRs: %d\n",        header->NSCOUNT);
        printf("Additional RRs: %d\n",       header->ARCOUNT);






        //memcpy(question, buf + sizeof(struct Header), sizeof(struct Question) ); //sizeof(struct Question) * header->QDCOUNT

        
       
        

        char *hostname_resp = NULL;

        //struct Question *question = (struct Question*)(buf);


        decode_hostname(buf, &hostname_resp);
        printf("Hostname (response): %s\n", hostname_resp);



        //struct Header *header = malloc(sizeof(struct Header));
        
        //memcpy(header, buf, sizeof(struct Header));




        
        //create a reply struct
        //and then send it


        break;
        if(dns_send(buf, sizeof(buf)) != 0)
        {
            exit(1);
        }

    }


    // struct Message_Response SGA = {
    //     .header = {
    //         .ID         = 0x700b,   // varies everytime
    //         .QR         = 0x01,     // It's a response
    //         .OPCODE     = 0x00,     // Standard query (0)
    //         .AA         = 0x00,     // Server is not an authority for domain
    //         .TC         = 0x00,     // Message is not truncated
    //         .RD         = 0x01,     // Do query recursively
    //         .RA         = 0x01,     // Server can do recursive queries
    //         .Z          = 0x00,
    //         //0x00                  // Answer authenticated: Answer/authority portion was not authenticated by the server
    //         //0x00                  // Non-authenticated data: Unacceptable
    //         .RCODE      = 0x00,     // No error
    //         .QDCOUNT    = 0x01,     // Questions: 1
    //         .ANCOUNT    = 0x01,     // Resource records in answer section
    //         .NSCOUNT    = 0x00,     // Server resource records in the authority records section
    //         .ARCOUNT    = 0x01      // Resource records in the additional records section
    //     },
    //     .question = {
    //         .QNAME      = "googlecom",
    //         .QTYPE      = 0x01,         // Type A
    //         .QCLASS     = 0x01          // Class: IN (0x0001)
    //     },
    //     .answer = {
    //         .NAME       = "googlecom",
    //         .TYPE       = 0x01,         // Type A
    //         .CLASS      = 0x01,         // Class: IN (0x0001)
    //         .TTL        = 0x54,         // 84 seconds
    //         .RDLENGTH   = 0x04,         // length of RDATA
    //         .RDATA      = "0xacd9a8ae"  // 4 octet ARPA Internet address
    //     },
    //     .additional = {
    //             .NAME       = 0x00,     // <Root>
    //             .TYPE       = 0x29,     // OPT 41
    //             .CLASS      = 0x0200,   // UDP payload size
    //             //0x00                  // "Higher bits in extended RCODE: 0x00"
    //             //EDNS0 version: 0
    //             .TTL        = 0x00,     // Mot specified
    //             .RDLENGTH   = 0x0c,     // Length of RDATA
    //             .RDATA      = 0x00      // Not specified
    //         }
    // };
    //char reply[100];
    //create_reply(buffer, reply);





}


/*

await receive
analyse
either return the ip of http or make a dns query 
to return ip we must be able to create a packet for that ip
must be able to "just foward" the return of the 3rd party dns query


*/