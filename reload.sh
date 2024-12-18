#!/bin/bash

if gcc -o gettftp src/gettftp/*.c src/common/*.c -Werror -Wall -Wextra -fno-stack-protector -g && gcc -o puttftp src/puttftp/*.c src/common/*.c -Werror -Wall -Wextra -fno-stack-protector -g; 
    then ./gettftp "$@" && ./puttftp "$@"
else
    exit 1
fi
    