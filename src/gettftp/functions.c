#include "functions.h"

ssize_t build_request(int opcode, char *file, char *mode, char *request_buffer){
    if (!file || !mode || !request_buffer) {
        fprintf(stderr, "Erreur : paramètres invalides\n");
        return -1;
    }
    size_t offset = 0;
    // initialisation of the opcode
    uint16_t opcode_unint = htons(opcode);
    // copy the opcode_unint in the request_buffer buffer
    memcpy(request_buffer+offset,&opcode_unint,sizeof(opcode_unint));
    offset += sizeof(opcode_unint);
    // copy the file name in the request_buffer buffer
    memcpy(request_buffer+offset,file,strlen(file));
    offset += strlen(file);
    // copy the null byte in the request_buffer buffer
    memcpy(request_buffer+offset,"\0",1);
    offset += 1;
    // copy the mode in the request_buffer buffer
    memcpy(request_buffer+offset,MODE,strlen(MODE));
    offset += strlen(MODE);
    // copy the null byte in the request_buffer buffer
    memcpy(request_buffer+offset,"\0",1);
    offset += 1;
    return offset;
}
void print_request(const char *buffer, size_t length) {
    printf("Requête formée (en hexadécimal) :\n");
    for (size_t i = 0; i < length; i++) {
        printf("%02x ", (unsigned char)buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n"); // Sauter une ligne tous les 16 octets
    }
    printf("\n");
}