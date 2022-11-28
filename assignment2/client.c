#include"protocol.h"
#define INPUT "request.txt"
    
// Driver code 
int main() { 
    int sockfd; 
    struct sockaddr_in     servaddr; 
    struct timeval timeout;
    
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    timeout.tv_sec = ACK_TIMER;
    timeout.tv_usec=0;
    //To manipulate options at the socket level, the level parameter must be set to SOL_SOCKET
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout))<0){
        perror("failed to set timeout");
        exit(EXIT_FAILURE); 
    };
    memset(&servaddr, 0, sizeof(servaddr)); 
        
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
        
    int n, len; 
    int i=0;
    len=sizeof(servaddr);
    FILE *fp = fopen(INPUT, "r");
    char line[20];
    PACKET packet;
    packet.start_packet=START_PACKET;
    packet.end_packet=END_PACKET;
    packet.client_id=CLIENT_ID_MAX;
	if(fp == NULL)
	{
		printf("Cannot Open File!\n");
		exit(EXIT_FAILURE);
	}

	while(fgets(line, sizeof(line), fp) != NULL)
	{
		char * words=NULL;
		words = strtok(line," ");
		packet.subscriber =(uint32_t) (atoi(words));
		words = strtok(NULL," ");
		packet.technology =(uint8_t)(atoi(words));
		packet.length=sizeof(packet.subscriber)+sizeof(packet.technology);
        packet.type=ACC_PER;
        packet.segment_no=i;
		i++;
        printf("\n================Send Access Request!================\n");
        sendto(sockfd, (const PACKET *)&packet, sizeof(packet), 
        0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        PACKET res;
        memset(&res,0,sizeof(res));
        n = recvfrom(sockfd, (PACKET *)&res, sizeof(res),  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        for(int retry=0;retry<MAX_RETRY&&n<0;retry++){
            printf("================\n[Client resent]\n================\n"); 
            sendto(sockfd, (const PACKET *)&packet, sizeof(packet), 
        0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
            n = recvfrom(sockfd, (PACKET *)&res, sizeof(res),  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        }
        if(n<0){
            printf("Server does not respond\n");
            close(sockfd);
            return 0;
        }
        print_packet(res); 

	}
	fclose(fp);
    close(sockfd); 
    return 0; 
}