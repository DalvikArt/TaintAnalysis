CC_C = gcc
CC_CPP = g++

overflow : overflow.c uaf
	$(CC_C) -fno-stack-protector -o overflow overflow.c

uaf : uaf.cpp
	$(CC_CPP) -fno-stack-protector -o uaf uaf.cpp
