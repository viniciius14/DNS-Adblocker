#include "hosts.h"

struct Line{
    int ip1, ip2, ip3, ip4;
    char *host_name;
};




char *read_line(char *host){

    FILE *fp = fopen("../Hosts/hosts.txt","r");


    struct Line line;
    char buffer[250];
    fgets(buffer, sizeof(buffer), fp);
    if(buffer[0] != '#'){
        sscanf(buffer, "%.d.%d.%d.%d %[^\n] \n", line.ip1, line.ip2, line.ip3, line.ip4, line.host_name);
    }
    else{
        memset(buffer, 0, sizeof(buffer));
        return NULL;
    }

    char a[] ={(char)line.ip1,'.',(char)line.ip2,'.',(char)line.ip3,'.',(char)line.ip4};




}