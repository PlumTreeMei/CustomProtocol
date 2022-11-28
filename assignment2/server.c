#include"protocol.h"
#define DATABASE "verification_database.txt"
int read_data(SUBSCRIBER data[]){
    char line[20];
    int i=0;
    FILE *fp = fopen(DATABASE, "r");
	if(fp == NULL)
	{
		printf("Cannot Open Verification Database!\n");
		return 0;
	}
	while(fgets(line, sizeof(line), fp) != NULL)
	{
		char * words=NULL;
		words = strtok(line," ");
		data[i].subscriber =(uint32_t) (atoi(words));
		words = strtok(NULL," ");
		data[i].technology = (uint8_t)(atoi(words));
		words = strtok(NULL," ");
		data[i].paid= atoi(words);
		i++;
	}
	fclose(fp);
    return i;
}  
// Driver code 
int main() { 
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
    SUBSCRIBER data[DATABASE_MAX];
    int size=read_data(data);
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
        
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
        
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    int len, n; 
    len = sizeof(cliaddr);  //len is value/result 
    while(1){
        PACKET packet;   
        memset(&packet,0,sizeof(packet));
        n = recvfrom(sockfd, (PACKET *)&packet, sizeof(packet),  
                0, ( struct sockaddr *) &cliaddr, &len); 
        printf("[Server received]\n================\n");
        print_packet(packet);
        PACKET_TYPE type=validate(data,size,packet);
        PACKET res=create_packet(type,packet.technology,packet.segment_no,packet.length,packet.subscriber);
        sendto(sockfd, (const PACKET*)&res, sizeof(res),  
        0, (const struct sockaddr *) &cliaddr, len); 
        
    }
    
    return 0; 
}

    
