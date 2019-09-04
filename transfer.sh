#!/bin/bash

cmd_str="scp main.cpp Makefile raspberryserver.h raspberryserver.cc raspberry.h raspberry.cpp pi@192.168.1.184:~/programs"
output=$(eval "$cmd_str")
echo "$output"
