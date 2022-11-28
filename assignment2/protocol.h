#ifndef PROTOCOL
#define PROTOCOL
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT  8080 
#define START_PACKET 0xFFFF
#define END_PACKET 0xFFFF
#define CLIENT_ID_MAX 0xFF
#define MAX_LENGTH 0xFF
#define ACK_TIMER 3
#define MAX_RETRY 3
#define SUBSCRIBER_MAX 0xFFFFFFFF
#define DATABASE_MAX 100
typedef enum PACKET_TYPE{
    ACC_PER=0XFFF8,
    NOT_PAID,
    NOT_EXIST,
    ACC_OK
}PACKET_TYPE;
typedef struct PACKET
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE type;
    uint8_t segment_no;
    uint8_t length;
    uint8_t technology;
    uint32_t subscriber;
    uint16_t end_packet;

}PACKET;
typedef struct SUBSCRIBER
{
    uint8_t technology;
    uint32_t subscriber;
    int paid;
}SUBSCRIBER;

PACKET create_packet(PACKET_TYPE t,uint8_t tech,uint8_t seg,uint8_t len,uint32_t sub);
PACKET_TYPE validate(SUBSCRIBER data[], int size,PACKET packet);
void print_packet(PACKET packet);
void print_type(PACKET_TYPE type);
#endif