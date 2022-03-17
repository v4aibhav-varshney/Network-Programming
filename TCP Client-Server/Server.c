#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define MAX 1024
#define PORT 4444

void chat(int sock_fd){
    char buffer[MAX] = {0} ;
    char name[MAX] ;
    int valread = 0 ;

    strcpy(buffer,"Hello , what is your name ?") ;
	send(sock_fd,buffer,strlen(buffer),0) ; 
    printf("Server : %s \n",buffer) ;
    bzero(buffer,sizeof(buffer)) ;

    valread = read(sock_fd,buffer,MAX) ;
    printf("Client : %s \n",buffer) ;
    strcpy(name,buffer) ;

    char ch = buffer[0] ;
    bzero(buffer,sizeof(buffer)) ;

    if(isupper(ch)){
        //First alphabet is uppercase letter
        strcpy(buffer,"200 OK") ;
    }
    else{
        //First alphabet is lowercase letter
        strcpy(buffer,"500 ERROR") ;
    }
    send(sock_fd,buffer,strlen(buffer),0) ; 
    printf("Server : %s \n",buffer) ;
    bzero(buffer,sizeof(buffer)) ;

    valread = read(sock_fd,buffer,MAX) ;
    printf("Client : %s \n",buffer) ;

    while(1){
        if(strcmp(buffer,".")==0){
            break ;
        }

        bzero(buffer,sizeof(buffer)) ;
        valread = read(sock_fd,buffer,MAX) ;
        printf("Client : %s \n",buffer) ;

    }

    bzero(buffer,sizeof(buffer)) ;
    char message []= "Thank you " ;
    strcat(message,name) ;
    strcpy(buffer,message) ;
	send(sock_fd,buffer,strlen(buffer),0) ; 
    printf("Server : %s \n",buffer) ;

}

int main(int argc, char const *argv[])
{
	int server_fd, new_sock;
	struct sockaddr_in address; //Sever address 

    //Network socket creation 
	if((server_fd = socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("Socket failure") ;
        exit(EXIT_FAILURE) ;
    }

    //Server address specifications 
	address.sin_family = AF_INET ;
	address.sin_addr.s_addr = INADDR_ANY ;
	address.sin_port = htons(PORT) ;

    //Socket binding to the port 
	if(bind(server_fd,(struct sockaddr *) &address ,sizeof(address))<0){
        perror("Bind failure !") ;
        exit(EXIT_FAILURE) ;
    }

    //Listening for connections 
	if(listen(server_fd,5)<0) {
        perror("Listen failure") ;
        exit(EXIT_FAILURE) ;
    }

    printf("Listening \n") ;

    //Accepting the connection 
    int addrlen = sizeof(address) ;
	if((new_sock = accept(server_fd,(struct sockaddr *) &address,(socklen_t* )&addrlen ))<0) {
        perror("Accept failure") ;
        exit(EXIT_FAILURE) ;
    } 

    printf("Accepted \n\n") ;

    //Communication with client 
    chat(new_sock) ;

	//Closing sockets
    printf("\nClosing connection \n") ;
	close(new_sock) ;
	close(server_fd) ;
    
    return 0;
}
