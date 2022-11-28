#include"protocol.h"

    
// Driver code 
int main() { 
    int sockfd; 
    char *client = "client"; 
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
        
    int n, i,len,status; 
    len=sizeof(servaddr);
    printf("Five Correct demo\n");
    for(int p = 1;p <=10;p++){
        DATA_PACKET packet=create_data_packet(p,client);
        if(p<6){
            i=p;
        }
        else if(p==6){
            printf("Error handling demo\n");
            i=0;
            packet.segment_no=i;
        }
        else if(p==7){
            i=2;
            packet.segment_no=i;
        }
        else if(p==8){
            i=1;
            packet.segment_no=i;
            packet.length=packet.length-1;
        }
        else if (p==9)
        {
            i=2;
            packet.segment_no=i;
            packet.end_packet=0xFFF0;
        }
        else if(p==10){
            i=2;
            packet.segment_no=i;
        }
        sendto(sockfd, (const DATA_PACKET *)&packet, sizeof(packet), 
        0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        n = recvfrom(sockfd, (int *)&status, sizeof(status),  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
        for(int retry=0;retry<MAX_RETRY&&n<0;retry++){
            printf("================\n[Client resent]\n================\n"); 
            sendto(sockfd, (const DATA_PACKET *)&packet, sizeof(packet), 
        0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
            n = recvfrom(sockfd, (int *)&status, sizeof(status),  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
        }
        if(n<0){
            printf("Server does not respond\n");
            close(sockfd);
            return 0;
        }
        n = recvfrom(sockfd, (int *)&status, sizeof(status),  
                MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
        printf("================\n[Client received]\n================\n"); 
        if(status==0){
            ACK_PACKET ack=create_ack_packet(i);
            print_ack_packet(ack);
        }else{
            REJECT_PACKET reject=create_reject_packet(status,i);
            print_rej_packet(reject);
        }
    }
    close(sockfd); 
    return 0; 
}