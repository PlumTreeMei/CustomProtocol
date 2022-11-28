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
#define CLIENT_ID 0xFF
#define MAX_LENGTH 0xFF
#define ACK_TIMER 3
#define MAX_RETRY 3
#define PAYLOAD_MAX 255
typedef enum PACKET_TYPE{
    DATA=0XFFF1,
    ACK,
    REJECT
}PACKET_TYPE;
typedef enum REJECT_SUB{
    OUT_OF_SEQUENCE=0XFFF4,
    LENGTH_MISMATCH,
    PACKET_MISSING,
    DUPLICATE_PACKET
}REJECT_SUB;
typedef struct DATA_PACKET
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE DATA;
    uint8_t segment_no;
    uint8_t length;
    char payload[PAYLOAD_MAX];
    uint16_t end_packet;
}DATA_PACKET;
typedef struct ACK_PACKET
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE ACK;
    uint8_t received_segment_no; 
    uint16_t end_packet;
}ACK_PACKET;
typedef struct REJECT_PACKET
{
    uint16_t start_packet;
    uint8_t client_id;
    PACKET_TYPE REJECT;
    REJECT_SUB reject_sub_code;
    uint8_t received_segment_no;
    uint16_t end_packet;
}REJECT_PACKET;
DATA_PACKET create_data_packet(uint8_t segment_no,char *payload);
ACK_PACKET create_ack_packet(uint8_t received_segment_no);
REJECT_PACKET create_reject_packet(REJECT_SUB reject_sub_code,uint8_t received_segment_no);
int validate_packet(DATA_PACKET packet,uint8_t expected_no);
void print_data_packet(DATA_PACKET packet);
void print_ack_packet(ACK_PACKET packet);
void print_rej_packet(REJECT_PACKET packet);
void print_rej_sub(REJECT_SUB reject_sub_code);
#endif