#include "puttftp.h"
#include "functions.h"

int main(int argc, char *argv[]) { 
    char * host_address = argv[1];
    char * host_port = argv[2];
    char * file = argv[3];

    if (argc < MIN_ARGUMENT || argc > MAX_ARGUMENT) {
        fprintf(stderr,"Wrong number of arguments for %s", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}