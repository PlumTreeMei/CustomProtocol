#include"protocol.h"
PACKET create_packet(PACKET_TYPE t,uint8_t tech,uint8_t seg,uint8_t len,uint32_t sub){
	PACKET packet;
	packet.start_packet=START_PACKET;
	packet.client_id=CLIENT_ID_MAX;
	packet.type=t;
	packet.segment_no=seg;
	packet.length=len;
	packet.technology=tech;
	packet.subscriber=sub;
	packet.end_packet=END_PACKET;

	return packet;
}
void print_packet(PACKET packet){
    printf("Details of Packet:\n");
	printf("Start of Packet ID: 0x%X\n",packet.start_packet);
	printf("Client ID: 0x%X \n",packet.client_id);
	printf("Segment No: %d\n",packet.segment_no);
	printf("Length: 0x%X \n",packet.length);
	printf("Technology: %u G\n",packet.technology);
	printf("Source Subscriber No: %u\n",packet.subscriber);
	printf("End of Packet ID: 0x%X\n",packet.end_packet);
	print_type(packet.type);
};
PACKET_TYPE validate(SUBSCRIBER data[], int size,PACKET packet){
	for (int i = 0; i < size; i++)
    {
        if (data[i].subscriber == packet.subscriber &&
            data[i].technology == packet.technology)
        {
            if (data[i].paid)
                return ACC_OK;
            else
                return NOT_PAID;
        }
    }
    return NOT_EXIST;
};
void print_type(PACKET_TYPE type){
	switch(type)
    {
        case 0xFFF8 : printf("Access permission request packet\n");
                      break;
        case 0xFFF9 : printf("Subscriber has not paid\n");
                      break;
        case 0xFFFA : printf("Subscriber does not exist\n");
                      break;
        case 0xFFFB : printf("Subscriber permitted to access the network\n");
    }
};