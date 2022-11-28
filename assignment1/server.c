#include"protocol.h"

#define PORT     8080 
    
// Driver code 
int main() { 
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
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
    int expected=1;    
    int len, n; 
    len = sizeof(cliaddr);  //len is value/result 
    while(1){
        DATA_PACKET packet;   
        memset(&packet,0,sizeof(packet));
        n = recvfrom(sockfd, (DATA_PACKET *)&packet, sizeof(packet),  
                0, ( struct sockaddr *) &cliaddr, &len); 
        printf("[Server received]\n================\n");
        print_data_packet(packet);
        if(packet.segment_no==0){
            expected=0;
        }
        int status=validate_packet(packet,expected);
        if(status!=OUT_OF_SEQUENCE){
            expected=expected+1;
        }
        sendto(sockfd, (const int*)&status, sizeof(status),  
        0, (const struct sockaddr *) &cliaddr, len); 
        
    }
    
    return 0; 
}

    
