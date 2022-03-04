#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define PORT 4444

int main(int argc, char const *argv[])
{
	int server_fd, new_sock;
	struct sockaddr_in address;
	char *hello = "Hello World";

    //Network socket creation 
	if((server_fd = socket(AF_INET,SOCK_STREAM,0))==0) {
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

    printf("Accepted \n") ;

    //Sending message to the client 
	send(new_sock,hello,strlen(hello),0) ; 
	printf("Hello message sent to browser \n");

	//Closing sockets
	close(new_sock) ;
	close(server_fd) ;
    
    return 0;
}
