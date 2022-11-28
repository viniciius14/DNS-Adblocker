#include "hosts.h"

struct Line{
    int ip1, ip2, ip3, ip4;
    char *host_name;
};




void read_line(char *host){

    FILE *fp = fopen("../Hosts/hosts.txt","r");


    struct Line line;
    char buffer[250];
    fgets(buffer, sizeof(buffer), fp);
    if(buffer[0] != '#'){
        sscanf(buffer, "%.d.%d.%d.%d %[^\n] \n", line.ip1, line.ip2, line.ip3, line.ip4, line.host_name);
    }
    else{
        memset(buffer, 0, sizeof(buffer));
    }


}