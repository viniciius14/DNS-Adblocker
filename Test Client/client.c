#include "../Headers/dns_utils.h"



int main(void){
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



    unsigned char reply[] = {
        0xc0, 0x32, 0x81, 0x80, 0x00,
        0x01, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x01, 0x06, 0x67, 0x6f,
        0x6f, 0x67, 0x6c, 0x65, 0x03,
        0x63, 0x6f, 0x6d, 0x00, 0x00, 
        0x01, 0x00, 0x01, 0xc0, 0x0c, 
        0x00, 0x01, 0x00, 0x01, 0x00, 
        0x00, 0x00, 0x73, 0x00, 0x04, 
        0x8e, 0xfa, 0xb8, 0x0e, 0x00, 
        0x00, 0x29, 0x02, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00
    };


    if(dns_send(SGQ, sizeof(struct Message_Query)) != 0)
    {
        exit(1);
    }





    char buffer[100];
    if(await_receive(buffer) != 0)
    {
        exit(1);
    } 

}

