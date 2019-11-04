#!/bin/bash

cmd_str="scp server/setup.txt server/main_server.cpp server/Makefile server/raspberryserver.h server/raspberryserver.cc server/raspberry.h server/raspberry.cpp server/connectclient.h server/connectclient.cc pi@192.168.1.184:~/programs"
output=$(eval "$cmd_str")
echo "$output"
