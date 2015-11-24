
# Makefile for "p2c", the Pascal to C translator.

CC  = gcc
P2C = p2c -LTURBO
INC = c:/emx/include/p2c.h
LIB = c:/emx/lib/libp2c.a


default: comp


# Translating mwave:

trans: mwave.c

mwave.c: mwave.pas
	$(P2C) mwave.pas

# Compiling :

comp:  mwave

mwave: mwave.c
     $(CC) -c -O2 mwave.c
	$(CC) -o mwave.exe mwave.o $(LIB) -I$(INC)

