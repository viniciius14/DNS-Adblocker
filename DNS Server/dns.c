#include "../Headers/dns_utils.h"


int main(void){

    //struct Message_Query *buf = (struct Message_Query*)malloc(sizeof(struct Message_Query));

    unsigned char buf[100];

    if(await_receive(buf) != 0)
    {
        exit(1);
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
    //         .QTYPE      = 0x01,     // Type A
    //         .QCLASS     = 0x01      // Class: IN (0x0001)
    //     },
    //     .answer = {
    //         .NAME       = "googlecom",
    //         .TYPE       = 0x01,         // Type A
    //         .CLASS      = 0x01,         // Class: IN (0x0001)
    //         .TTL        = 0x54,         // 84 seconds
    //         .RDLENGTH   = 0x04,         // length of RDATA
    //         .RDATA      = "0xacd9a8ae"    // 4 octet ARPA Internet address
    //     },
    //     .additional = {
    //             .NAME       = 0x00,     // <Root>
    //             .TYPE       = 0x29,     // OPT 41
    //             .CLASS      = 0x0200,   // UDP payload size
    //             //0x00                  // "Higher bits in extended RCODE: 0x00"
    //             //EDNS0 version: 0
    //             .TTL        = 0x00,     // Mot specified
    //             .RDLENGTH   = 0x0c,     // Length of RDATA
    //             .RDATA      = 0x00      // Mot specified
    //         }
    // };
    //char reply[100];
    //create_reply(buffer, reply);




    if(dns_send(buf, sizeof(buf)) != 0){
        exit(1);
    }
}


/*

await receive
analyse
either return the ip of http or make a dns query 
to return ip we must be able to create a packet for that ip
must be able to "just foward" the return of the 3rd party dns query


*/











// void* last;

// printf("sizeof array   -----> %ld\n",sizeof(reply));


// for(int i = 0 ; i != sizeof(reply) ; i++){
//     printf("%p -----> %x \n", &(reply[i]), reply[i]);
//     last = &reply[i];
// }

// foo(reply, sizeof(reply));



// void *foo(unsigned char *reply, size_t size){
//     void* last;

//     printf("sizeof array   -----> %ld\n",sizeof(reply));/// not the actual size, sizeof pointer


//     for(int i = 0 ; i != size; i++){
//         printf(" -----> %x \n",  reply[i]);
//         last = &reply[i];
//     }

//     unsigned char n_reply[size];

//     printf("\n\n\n");
//     for(int i = 0 ; i != size ; i++){
// 		n_reply[i] = reply[i];
// 	}

//     for(int i = 0 ; i != size; i++){
//         printf(" -----> %x \n", n_reply[i]);
//     }

// }



