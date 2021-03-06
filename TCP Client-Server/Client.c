#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 4444

void readline(char * str,int size,char delim){
    int i = 0 ;
    char ch = getchar() ;
    while(ch!=delim){
        if(i==size-1){ //To save space for null character 
            break ;
        }
        str[i] = ch ;
        ch = getchar() ;
        i++ ;
    }
    str[i] = '\0' ; //Null character 
    return ;
}

void chat (int sock_fd){
    char buffer[MAX] = {0} ;
    int valread = 0 ;

    valread = read(sock_fd,buffer,MAX) ;
    printf("Server : %s \n",buffer) ; 
    bzero(buffer,sizeof(buffer)) ;

    printf("Enter name : ") ;
    readline(buffer,sizeof(buffer),'\n') ;

	send(sock_fd,buffer,strlen(buffer),0) ; 
    printf("Client : %s \n",buffer) ;
    bzero(buffer,sizeof(buffer)) ;

    valread = read(sock_fd,buffer,MAX) ;
    printf("Server : %s \n",buffer) ;

    if(strcmp(buffer,"200 OK")==0){
        while(strcmp(buffer,".")!=0){
            printf("Enter message : ") ;
            bzero(buffer,sizeof(buffer)) ;

            readline(buffer,sizeof(buffer),'\n') ;

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

}

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
    chat(sock_fd) ;

	//Closing sockets
    printf("\nClosing connection \n") ;
	close(sock_fd) ;
    
    return 0;
}
