#include "common.h"

int get_servers_infos(char * host_address,char * host_port, struct addrinfo ** result_infos) {
    struct addrinfo filter;
    
    // filter initialization
    memset(&filter,0,sizeof(filter));
    filter.ai_family = AF_INET; // IPv4
    filter.ai_socktype = SOCK_DGRAM; // UDP
    filter.ai_protocol = IPPROTO_UDP; // UDP

    int exec_status = getaddrinfo(host_address, host_port, &filter, result_infos);
    return exec_status;
}

int create_socket(struct addrinfo * server_infos) {
    int socket_file_descriptor = socket(server_infos->ai_family, server_infos->ai_socktype, server_infos->ai_protocol);
    return socket_file_descriptor;
}

void print_request(const char *buffer, size_t length) {
    printf("Requête formée (en hexadécimal) :\n");
    for (size_t i = 0; i < length; i++) {
        printf("%02x ", (unsigned char)buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n"); // Sauter une ligne tous les 16 octets
    }
    printf("\n");
}    
    