#ifndef HOSTS_C
#define HOSTS_C
#include "../Headers/hosts.h"



char *find_host(const char *hostname)
{
    FILE *fp = fopen("../Hosts/hosts.txt","r");
    if(!fp) goto error;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

    while(1){
        char temp[250];
        char *ip, *temp_host, *token, delim[] = " \t\n";

        if(ftell(fp) == size) break;

        fgets(temp, sizeof(temp), fp);
        if(!fgets) goto error;

        if(temp[0] == '#' || temp[0] == '\n')continue;
        
        char *ptr = strtok(temp, delim);
        ip = ptr;
        ptr = strtok(NULL, delim);
        temp_host = ptr;
        
        if(!strcmp(temp_host, hostname)){
            return ip;
        }
    }

error:
    fclose(fp);
    return NULL;  
}





#endif