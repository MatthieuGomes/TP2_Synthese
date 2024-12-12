#include "gettftp.h"
#include "functions.h"

int main(int argc, char *argv[]) { 
    struct addrinfo *result,*pointer;
    // structs initialization
    memset(&result,0,sizeof(result));
    memset(&pointer,0,sizeof(pointer));

    char * host_address = argv[1];
    char * host_port = argv[2];
    char * file = argv[3];

    if (argc < MIN_ARGUMENT || argc > MAX_ARGUMENT) {
        fprintf(stderr,"Wrong number of arguments for %s\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (file != NULL){
        printf("file: %s\n", file);
    }
    
    int exec_status = get_servers_infos(host_address,host_port,&result);
    if(exec_status != EXIT_SUCCESS){
        fprintf(stderr,"Error in get_servers_infos\n");
        return EXIT_FAILURE;
    }
    int number_of_servers = 0;
    // goes through the addrinfo linked list to count the number of servers
    for(pointer = result; pointer != NULL; pointer = pointer->ai_next){
        number_of_servers++;
    }
    // checks the number of servers
    switch (number_of_servers)
    {
    case 0:
        fprintf(stderr,"No servers found\n");
        return EXIT_FAILURE;
    case 1:
        break;
    default:
        fprintf(stderr,"%d servers : Too much servers found\n",number_of_servers);    
        return EXIT_FAILURE;
    }
    
    // gets the IP address of the server
    struct sockaddr_in * sock_address = (struct sockaddr_in *) result->ai_addr;
    void * ip_addres_from_sock = &(sock_address->sin_addr);
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, ip_addres_from_sock, ip_address, INET_ADDRSTRLEN);
    printf("IP address: %s\n", ip_address);

    // creates socket for UDP server
    int socket_file_descriptor = create_socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socket_file_descriptor < SOCKET_SUCCESS_LIMIT){
        fprintf(stderr,"Error in socket creation, socket returned %dn\n",socket_file_descriptor);
        return EXIT_FAILURE;
    }
    else{
        printf("Socket created witd fd: %d\n",socket_file_descriptor);
    }

    freeaddrinfo(result);
    freeaddrinfo(pointer);
    return EXIT_SUCCESS;
}