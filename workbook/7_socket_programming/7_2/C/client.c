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
    if (argc < 2) {
       fprintf(stderr,"usage %s server name\n", argv[0]);
       exit(0);
    }
    portno = 2525;
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
        
    bzero(buffer, 256);

    /*
        recupero hostname usando la funzione gethostname(buf, size)
        - buf: array di caratteri in cui sarà scritto l'hostname (char *)
        - size: dimensione del buffer (size = sizeof(buf))
    */
    
    gethostname(buffer, sizeof(buffer));

    char message[1024]; // conterrà il messaggio di benvenuto

    bzero(message, 1024); // inizializzo a 0 

    // compongo il messaggio di benvenuto

    char* greeting = "Welcome from ";

    strcpy(message, greeting);

    strcat(message, buffer);

    // invio messaggio di benvenuto

    n = write(sockfd,message,sizeof(message));

    /*
        se write ritorna un valore < 0 stampo un messaggio di errore
    */
    if (n < 0) 
        error("ERROR writing to socket");
    
    return 0; 
}
