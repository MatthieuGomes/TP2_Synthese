#include "gettftp.h"
#include "functions.h"

int main(int argc, char *argv[]) { 
    struct addrinfo *server_infos,*pointer;
    // structs initialization
    memset(&server_infos,0,sizeof(server_infos));
    memset(&pointer,0,sizeof(pointer));

    char * host_address = argv[1];
    char * host_port = argv[2];
    char * file = argv[3];

    if (argc < MIN_ARGUMENT || argc > MAX_ARGUMENT) {
        printf("Wrong number of arguments for %s\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (file != NULL){
        printf("file: %s\n", file);
    }
    
    int exec_status = get_servers_infos(host_address,host_port,&server_infos);
    if(exec_status != EXIT_SUCCESS){
        perror("Error in get_servers_infos");
        return EXIT_FAILURE;
    }
    int number_of_servers = 0;
    // goes through the addrinfo linked list to count the number of servers
    for(pointer = server_infos; pointer != NULL; pointer = pointer->ai_next){
        number_of_servers++;
    }
    freeaddrinfo(pointer);
    // checks the number of servers
    

    switch (number_of_servers)
    {
    case 0:
        perror("No servers found");
        return EXIT_FAILURE;
    case 1:
        break;
    default:
        printf("%d servers : Too much servers found\n",number_of_servers);    
        return EXIT_FAILURE;
    }
    
    // gets the IP address of the server
    void * ip_addres_from_sock = &(((struct sockaddr_in *) server_infos->ai_addr)->sin_addr);
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, ip_addres_from_sock, ip_address, INET_ADDRSTRLEN);
    printf("IP address: %s\n", ip_address);

    // creates sockets
    int server_socket = socket(server_infos->ai_family, server_infos->ai_socktype, server_infos->ai_protocol);
    int client_socket = socket(server_infos->ai_family, server_infos->ai_socktype, server_infos->ai_protocol);
    if (server_socket < SOCKET_SUCCESS_LIMIT || client_socket < SOCKET_SUCCESS_LIMIT){
        printf("Error in socket creation, socket returned %dn\n",server_socket);
        return EXIT_FAILURE;
    }
    else{
        printf("Socket created witd fd: %d\n",server_socket);
    }
    // config socket 
    struct sockaddr * server_address =server_infos->ai_addr;
    
    struct sockaddr_in client_sock_address;
    memset(&client_sock_address,0,sizeof(client_sock_address));
    client_sock_address.sin_family = AF_INET;
    client_sock_address.sin_port = htons(CLIENT_PORT);
    client_sock_address.sin_addr.s_addr = inet_addr(LOCALHOST);
    struct sockaddr * client_address = (struct sockaddr *) &client_sock_address;
    socklen_t server_address_length = server_infos->ai_addrlen;

    // connect/bind sockets to the server
    int connection_status = connect(server_socket, server_address, sizeof(struct sockaddr));
    if (connection_status == CONNECTION_FAILURE){
        perror("Error in connection");
        return EXIT_FAILURE;
    }
    else{
        printf("Connection established\n");
    }
    int bind_status = bind(client_socket, client_address, sizeof(struct sockaddr));
    if (bind_status == CONNECTION_FAILURE){
        perror("Error in binding");
        return EXIT_FAILURE;
    }
    else{
        printf("Binding established\n");
    }

    char * request_buffer = malloc(MAX_BUFFER_SIZE);
    memset(request_buffer,0,sizeof(MAX_BUFFER_SIZE));
    ssize_t request_size = build_request(RRQ_CODE,file,MODE,request_buffer);
    if ((int) request_size == REQUEST_BUILDING_ERROR){
        perror("Error in request building\n");
        return EXIT_FAILURE;
    }
    print_request(request_buffer,request_size);

    // send the request to the server
    ssize_t send_status = sendto(client_socket,request_buffer,request_size,0,server_address,sizeof(struct sockaddr));
    if((int) send_status == SEND_FAILURE){
        perror("Error in sending the request");
        return EXIT_FAILURE;
    }
    else{
        printf("Request sent\n");
    }

    // receive the response from the server
    char * response_buffer = malloc(MAX_BUFFER_SIZE);
    memset(response_buffer,0,sizeof(MAX_BUFFER_SIZE));
    ssize_t recv_status = 0;
    char * file_path = malloc(MAX_BUFFER_SIZE);
    sprintf(file_path,"./received/%s",file);
    int new_file = creat(file_path,766);
    do{
        recv_status= recvfrom(client_socket,response_buffer,MAX_BUFFER_SIZE,0,server_address,&server_address_length); // changes the port number to the newly opened port by the server for correct communication
        if((int) recv_status == SEND_FAILURE){
            perror("Error in receiving the response");
            int deletion_status= unlink(file_path);
            if(deletion_status==EXIT_FAILURE){
                perror("Error in removing unfinished file");
            }
            return EXIT_FAILURE;
        }
        else{
            printf("Response received\n");
            printf("Response size : %ld\n",recv_status);
            print_request(response_buffer,recv_status);
        }
        uint16_t opcode;
        uint16_t block_number;
        char received_file[recv_status-2*sizeof(uint16_t)];
        memcpy(&opcode,&response_buffer[OP_CODE_POSITION],sizeof(opcode));
        memcpy(&block_number,&response_buffer[BLOCK_NUMBER_POSITION],sizeof(block_number));
        memcpy(&received_file,&response_buffer[DATA_POSITION],sizeof(received_file));
        printf("Opcode : %d\n",ntohs(opcode));
        printf("Block number : %d\n",ntohs(block_number));
        if(opcode==5){
            printf("Error code : %d\n",ntohs(block_number));
            int deletion_status= unlink(file_path);
            if(deletion_status==EXIT_FAILURE){
                perror("Error in removing unfinished file");
            }
            return EXIT_FAILURE;
        }
        int written_file_size = write(new_file,received_file,sizeof(received_file));
        if(written_file_size==EXIT_FAILURE){
            perror("Error in writting file");
            int deletion_status= unlink(file_path);
            if(deletion_status==EXIT_FAILURE){
                perror("Error in removing unfinished file");
            }
            return EXIT_FAILURE;
        }
        char * ack_buffer = malloc(MAX_BUFFER_SIZE);
        memset(ack_buffer,0,sizeof(ACK_SIZE));
        uint16_t ack_opcode = ntohs(ACK_CODE);
        uint16_t ack_block_number = ntohs(block_number);
        memcpy(ack_buffer,&ack_opcode,sizeof(ack_opcode));  
        memcpy(&ack_buffer[BLOCK_NUMBER_POSITION_IN_ACK],&ack_block_number,sizeof(ack_block_number));
        print_request(ack_buffer,ACK_SIZE);
        //Send the acknowledgment to the server
        ssize_t ack_send_status = sendto(client_socket,ack_buffer,ACK_SIZE,0,server_address,sizeof(struct sockaddr));
        if((int) ack_send_status == SEND_FAILURE){
            perror("Error in sending the acknowledgment");
            int deletion_status= unlink(file_path);
            if(deletion_status==EXIT_FAILURE){
                perror("Error in removing unfinished file");
            }
            return EXIT_FAILURE;
            
        }
        else{
            printf("Acknowledgment sent\n");
        }
    
    }while((int)recv_status>=PACKET_MAX_SIZE);
    // extract the opcode from the response
    
    
    // acknowledge request building
    

    free(request_buffer);
    close(client_socket);
    close(server_socket);
    freeaddrinfo(server_infos);
    return EXIT_SUCCESS;
}