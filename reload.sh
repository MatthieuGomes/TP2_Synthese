#!/bin/bash

gcc -o enseash src/*.c -Werror -Wall -Wextra -fno-stack-protector -g 

if [ $? -eq 0 ] 
    then ./enseaserver
else
    exit 1
fi
    