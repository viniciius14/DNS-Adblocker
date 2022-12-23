#include "../Headers/hosts.h"



char *find_host(const char *hostname)
{
    FILE *fp = fopen("../hosts/hosts.txt","r");
    if(!fp) goto error;

    while(1){
        char temp[250];
        char *ip, *temp_host, *token, delim[] = " \t\n";;
        
        fgets(temp, sizeof(temp), fp);
        if(!fgets) goto error;

        if(temp[0] == '#' || temp[0] == '\n')continue;
        
        char *ptr = strtok(temp, delim);
        ip = ptr;
        ptr = strtok(NULL, delim);
        temp_host = ptr;
        
        if(*temp_host == *hostname){
            return ip;
        }
    }

error:
    fclose(fp);
    return NULL;  
}

int main(){

    char *a = find_host("www.adverterenzeeland.nl");
    if(a){
        printf("found!\n%s",a);
    }
}