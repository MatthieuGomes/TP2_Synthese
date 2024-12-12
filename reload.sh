#!/bin/bash

gcc -o gettftp src/gettftp/*.c -Werror -Wall -Wextra -fno-stack-protector -g 
gcc -o puttftp src/puttftp/*.c -Werror -Wall -Wextra -fno-stack-protector -g 

if [ $? -eq 0 ] 
    then ./gettftp "$@" && ./puttftp "$@"
else
    exit 1
fi
    