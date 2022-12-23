/*
Header for interpretation of hosts file and all things hosts file related
*/
#ifndef HOSTS_H
#define HOSTS_H

#include <stdio.h>
char *read_line(char *host);
//ler a linha %.d.%d.%d.%d %s
//se a linha comecar com um # ignorar

//estrutura da cache
// 0.0.0.0 %s datetime ttl 
//must support addittion and addition of new lines

#endif