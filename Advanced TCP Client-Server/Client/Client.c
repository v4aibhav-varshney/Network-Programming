#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024

int main(int argc, char const *argv[])
{
	int sock_fd, new_sock;
	struct sockaddr_in address;

    char ipAddress[16];
    int PORT ;
    char filename[100] ;

    printf("Enter server's ip address : ") ;
    scanf("%s",ipAddress) ;
    printf("Enter server's port no. : ") ;
    scanf("%d",&PORT) ;
    printf("Enter file name : ") ;
    scanf("%s",filename) ;

    //Network socket creation 
	if((sock_fd = socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("Socket failure") ;
        exit(EXIT_FAILURE) ;
    }

    //Server address specifications 
	address.sin_family = AF_INET ;
    address.sin_addr.s_addr = inet_addr(ipAddress) ;
	address.sin_port = htons(PORT) ;

    //Connecting to the server 
    if(connect(sock_fd , (struct sockaddr *)&address, sizeof(address))<0){
        perror("Server not reachable ") ;
        exit(EXIT_FAILURE) ;
    }

    printf("Connected \n\n") ;

    //Communication with server 
    char buffer[MAX] = {0} ;
    
    char request[100] = "GET ";
    strcat(request,filename) ;
    printf("Client : %s \n",request) ;
    send(sock_fd,request,sizeof(request),0) ;

    bzero(buffer,sizeof(buffer)) ;

    read(sock_fd,buffer,sizeof(buffer)) ;    
    printf("Server : %s \n",buffer) ;

    //Write n bytes in the file : 
    FILE* fp = fopen(filename,"w") ;

    if(fp==NULL){
        printf("Unable to create file") ;
    }

    fputs(buffer,fp) ;

    fclose(fp) ;

    printf("\nClosing connection \n") ;

	//Closing sockets
	close(sock_fd) ;
    
    return 0;
}
