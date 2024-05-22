#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg){
    perror(msg);
    exit(0);
}

void send_http_request(int sockfd, char * pathname, char * hostname){
	char reqbuffer[256];
	int n;
	bzero(reqbuffer, 256); // when allocating variables on stack, there's no guarantee they're set to 0
    //assume URL doesn't exceed buffer's size
	sprintf(reqbuffer, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", pathname, hostname); // build request line and put it into reqbuffer
	n=write(sockfd, reqbuffer, strlen(reqbuffer));
	if (n<0){
        error("ERROR writing to socket");
    }
}

void read_http_response(int sockfd){
	char respbuffer[256];
	bzero(respbuffer, 256);
	read(sockfd, respbuffer, 255);
	printf("%s\n", respbuffer);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char * hostname; // hardcoded version hostname = "localhost"
    char * pathname;

    //char buffer[256];
    if (argc < 3) { // 2 if hostname hardcoded
       fprintf(stderr,"usage %s URL\n", argv[0]);
       exit(0);
    }
    pathname = argv[2]; // argv[1] if hostname hardcoded
	hostname = argv[1]; // not needed if hostname hardcoded
    portno = 8080; // alternative 80
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4 address
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno); // long (l) /short (s) numbers must be in network format 
    if (connect(sockfd,(const struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    send_http_request(sockfd, pathname, hostname); // hostname needed to build the correct header
    read_http_response(sockfd);    
    return 0;
}
