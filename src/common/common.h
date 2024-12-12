#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX_ARGUMENT 4
#define MIN_ARGUMENT 3
#define TRUE 1
#define FALSE 0
#define IPV4_ADDR_LEN 16
#define SOCKET_SUCCESS_LIMIT 0
#define CONNECTION_FAILURE -1
#define RRQ_CODE 1
#define MAX_BUFFER_SIZE 512
#define MODE "octet"
#define REQUEST_BUILDING_ERROR -1
#define SEND_FAILURE -1

int get_servers_infos(char * host_address,char * host_port, struct addrinfo ** result_infos);
int create_socket(struct addrinfo * server_infos);

#endif