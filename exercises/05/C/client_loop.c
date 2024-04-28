#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
/*
    contiene una libreria (resolver) che costituisce l'entry point per il sistema DNS (Domain Name System)
*/
#include <netdb.h> 

void error(char *msg){
    perror(msg);
    exit(0);
}
/*
    client deve essere invocato con hostname e numero di porta, per poter recuperare l'hostname serve convertire un nome simbolico in un indirizzo IP (si usa <netdb.h>)
*/
int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    /*
        server: array di elementi
    */
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    /*
        socket inizializzato esattamente come quello server-side, per cambiare il comportamento invoco funzioni diverse
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    /*
        gethostbyname: funzione che consente di risolvere il nome simbolico di un host (ritorna uno o piu' indirizzi IP associati al nome)
        quando una macchina ha molteplici indirizzi IP e li utilizza per bilanciare il traffico, il DNS ritorna gli indirizzi IP in ordine casuale (il client di solito
        prende il primo)
        i valori ritornati sono in formato network
    */
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    /*
        copio il campo h_addr (struttura che contiene un indirizzo IP) nel campo sin_addr.s_addr
    */
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    /*
        setto il numero di porta (numero di porta - intero - era in formato host, devo trasformarlo in formato network con htons)
    */
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(const struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    while(1){

    	printf("Please enter the message: ");
    	bzero(buffer, 256);
    /*
        leggo da stdin massimo 255 caratteri
    */
    	fgets(buffer, 255, stdin);
    /*
        avendo garantito che la stringa sia null-terminated posso calcolare quanti byte ha
    */
    	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0) 
        	error("ERROR writing to socket");
    /*
        riutilizzo il buffer (quindi lo setto di nuovo a 0)
    */
    	bzero(buffer,256);
    /*
        eseguo operazione di lettura per recuperare il messaggio mandato dal server
    */
    	n = read(sockfd,buffer,255);
    	if (n < 0) 
        	error("ERROR reading from socket");
    	printf("%s\n",buffer);
    }
    return 0;
}
