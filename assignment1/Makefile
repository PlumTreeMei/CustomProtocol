# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g		adds debugging information to the executable file
#  -Wall	turns on most, but not all, compiler warnings
#  -fshort-enums	so enum type has the smallest size possible to hold the largest enum value
CFLAGS  = -g -Wall -fshort-enums

# the build target executable:
HEADER = protocol
TARGET = client server noresponse


all: $(TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(HEADER).c

cs: client server

client: client.c
	$(CC) $(CFLAGS) -o client client.c $(HEADER).c

server: server.c
	$(CC) $(CFLAGS) -o server server.c $(HEADER).c

noresponse: noresponse.c
	$(CC) $(CFLAGS) -o noresponse noresponse.c $(HEADER).c
clean:
	$(RM) $(TARGET)