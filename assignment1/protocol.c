#include"protocol.h"
DATA_PACKET create_data_packet(uint8_t segment_no, char *payload){
    DATA_PACKET packet;
	packet.start_packet=START_PACKET;
    packet.client_id=CLIENT_ID;
    packet.segment_no=segment_no;
    packet.length=strlen(payload);
	packet.end_packet=END_PACKET;
    memset(packet.payload, 0, PAYLOAD_MAX);
    memcpy(packet.payload, payload, strlen(payload));
	return packet;
};
ACK_PACKET create_ack_packet(uint8_t received_segment_no){
	ACK_PACKET packet;
	packet.start_packet=START_PACKET;
	packet.client_id=CLIENT_ID;
	packet.received_segment_no=received_segment_no;
	packet.end_packet=END_PACKET;
	return packet;
};
REJECT_PACKET create_reject_packet(REJECT_SUB reject_sub_code,uint8_t received_segment_no){
	REJECT_PACKET packet;
	packet.start_packet=START_PACKET;
	packet.client_id=CLIENT_ID;
	packet.reject_sub_code=reject_sub_code;
	packet.received_segment_no=received_segment_no;
	packet.end_packet=END_PACKET;
	return packet;
};
int validate_packet(DATA_PACKET packet,uint8_t expected_no){
    if(packet.segment_no>0 && packet.segment_no<expected_no)
        return DUPLICATE_PACKET;
    if(packet.segment_no!= expected_no)
        return OUT_OF_SEQUENCE;
    if(packet.length != strlen(packet.payload))
        return LENGTH_MISMATCH;
    if(packet.end_packet != END_PACKET)
        return PACKET_MISSING;
    return 0;

}

void print_data_packet(DATA_PACKET packet){
    printf("Details of Packet:\n");
	printf("Start of Packet ID: 0x%X\n",packet.start_packet);
	printf("Client ID: 0x%X \n",packet.client_id);
	printf("Data: DATA \n" );
	printf("Segment No: %d\n",packet.segment_no);
	printf("Length: 0x%X \n",packet.length);
	printf("Payload: %s\n",packet.payload);
	printf("End of Packet ID: 0x%X\n",packet.end_packet);
};
void print_ack_packet(ACK_PACKET packet){
	printf("Details of Packet:\n");
	printf("Start of Packet ID: 0x%X\n",packet.start_packet);
	printf("Client ID: 0x%X \n",packet.client_id);
	printf("Data: ACK \n" );
	printf("Recieved segment No: %d\n",packet.received_segment_no);
	printf("End of Packet ID: 0x%X\n",packet.end_packet);
};
void print_rej_packet(REJECT_PACKET packet){
	printf("Details of Packet:\n");
	printf("Start of Packet ID: 0x%X\n",packet.start_packet);
	printf("Client ID: 0x%X \n",packet.client_id);
	printf("Data: REJECT \n" );
	printf("Reject sub code: 0x%X\n",packet.reject_sub_code);
	printf("Recieved segment No: %d\n",packet.received_segment_no);
	printf("End of Packet ID: 0x%X\n",packet.end_packet);
	print_rej_sub(packet.reject_sub_code);
};
void print_rej_sub(REJECT_SUB reject_sub_code){
	switch(reject_sub_code)
    {
        case 0xFFF4 : printf("ERROR: Received packet at server is not in sequence with expected packet\n");
                      break;
        case 0xFFF5 : printf("ERROR: This packet has a length mismatch error\n");
                      break;
        case 0xFFF6 : printf("ERROR: This packet has a missing end of packet identifier error\n");
                      break;
        case 0xFFF7 : printf("ERROR: Duplicate packet Received\n");
    }
};