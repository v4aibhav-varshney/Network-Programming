#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 4444

int main(int argc, char const *argv[])
{
	int sock_fd, new_sock;
	struct sockaddr_in address;

    //Network socket creation 
	if((sock_fd = socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("Socket failure") ;
        exit(EXIT_FAILURE) ;
    }

    //Server address specifications 
	address.sin_family = AF_INET ;
    address.sin_addr.s_addr = inet_addr("127.0.0.1") ;
	address.sin_port = htons(PORT) ;

    //Connecting to the server 
    if(connect(sock_fd , (struct sockaddr *)&address, sizeof(address))<0){
        perror("Connection failure") ;
        exit(EXIT_FAILURE) ;
    }

    printf("Connected \n\n") ;

    //Communication with server 
    char buffer[MAX] = {0} ;
    int valread = 0 ;

    valread = read(sock_fd,buffer,MAX) ;
    printf("Server : %s \n",buffer) ; 
    bzero(buffer,sizeof(buffer)) ;

    printf("Enter name : \n") ;
    int idx =0 ;
    while((buffer[idx++]=getchar())!='\n'){
        if(idx==MAX-1){
            break ;
        }
    }

	send(sock_fd,buffer,strlen(buffer),0) ; 
    printf("Client : %s \n",buffer) ;
    bzero(buffer,sizeof(buffer)) ;

    valread = read(sock_fd,buffer,MAX) ;
    printf("Server : %s \n",buffer) ;

    if(strcmp(buffer,"200 OK")==0){
        printf("Enter message : \n") ;
        while(strcmp(buffer,".\n")!=0){
            bzero(buffer,sizeof(buffer)) ;

            int idx =0 ;
            while((buffer[idx++]=getchar())!='\n'){
                if(idx==MAX-1){
                    break ;
                }
            }

            send(sock_fd,buffer,strlen(buffer),0) ; 
            printf("Client : %s \n",buffer) ;
        }

    }
    else if(strcmp(buffer,"500 ERROR")==0){
        bzero(buffer,sizeof(buffer)) ;
        strcpy(buffer,".") ;
        send(sock_fd,buffer,strlen(buffer),0) ; 
        printf("Client : %s \n",buffer) ;
    }


    bzero(buffer,sizeof(buffer)) ;
    valread = read(sock_fd,buffer,MAX) ;
    printf("Server : %s \n",buffer) ; 

    printf("\nClosing connection \n") ;

	//Closing sockets
	close(sock_fd) ;
    
    return 0;
}
