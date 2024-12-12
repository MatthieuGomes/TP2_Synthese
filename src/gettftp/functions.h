#ifndef GETTFTP_FUNCTIONS_H
#define GETTFTP_FUNCTIONS_H
#include "../common/common.h"

ssize_t build_request(int opcode, char *file, char *mode, char *request_buffer);
void print_request(const char *buffer, size_t length);


#endif