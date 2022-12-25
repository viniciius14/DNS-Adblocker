#include "../Headers/dns_utils.h"



int main(void){
    unsigned char new[] = {
        0x4e, 0x93, 0x01, 0x00, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x67,
        0x6f, 0x6f, 0x00, 0x10, 0x67, 0x6c, 0x65,
        0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01
    };

    if(dns_send(new, sizeof(new), "53", "127.0.0.1") != 0)
    {
        exit(1);
    }

    unsigned char buffer[100];
    if(await_receive(buffer, "53") != 0)
    {
        exit(1);
    } 

}

