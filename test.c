





#include "Headers/dns_utils.h"


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





int main(void){

    for(;;){

   
        char buffer[100];
        if(await_receive(buffer) == NULL)
        {
            exit(1);
        } 


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
        //char reply[100];
        //create_reply(buffer, reply);


    

        if(dns_send(reply, sizeof(reply)) == NULL){
            exit(1);
        }
    }
}