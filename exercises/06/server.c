/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
/*
    include standard
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*
    include specifici per le socket api
*/
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// stampa un messaggio di errore in caso di problema
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    /*
        sockfd: file descriptor della socket
        newsockfd: quello che ritorna accept
        portno: numero di porta
    */
    int sockfd, newsockfd, portno;
    /*
        socklen_t: tipo generico (di fatto unsigned int) lunghezza di una struttura dati che contiene una socket
    */
    socklen_t clilen;
    /*
        256 caratteri per il buffer scelta arbitraria (la maggior parte dei server lavora con buffer di 1024 caratteri)
    */
    char buffer[256];
    /*
        sockaddr_in: struct che contiene tutti i parametri rilevanti di un indirizzo di rete
    */
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    /*
        mi aspetto esattamente un parametro (il numero di porta)
    */
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    /*
        inizializzo la socket
        AF_INET: lavoro con IPv4
        SOCK_STREAM: lavoro con TCP
        protocol non viene utilizzato (0)
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*
        se non ho socket disponibili (ho saturato il numero di porte libere)
    */
    if (sockfd < 0) 
        error("ERROR opening socket");
    /*
        prendo il puntatore alla struttura serv_addr (di tipo sockaddr_in), lo casto a char * e riempio la struttura dati di zeri con bzero
        avendo allocato le struct sullo stack non ho garanzia che vengano settate a zero (vanno inizializzate)
    */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    /*
        leggo il numero di porta
    */
    portno = atoi(argv[1]);
    /*
        riempio la struttura dati
        sin_family: tipo di dato (in questo caso indirizzo IPv4)
        sin_addr.s_addr: setto l'indirizzo IP su cui eseguo la bind (se voglio accettare connessioni da tutto il mondo quando faccio listen voglio che appaia che sono in 
        ascolto su 0.0.0.0:port_number_specificato) -> 0.0.0.0 mappato nella macro INADDR_ANY
        sin_port: numero di porta (nel mondo del networking si lavora con una notazione binaria ben specifica, un intero potrebbe avere endianess diversa a seconda dell'host,
        quando mando i segmenti/datagram sulla rete endianess deve essere unica) -> funzioni hton/ntoh (hton: host to network, ntoh: network to host), hotns (s: short, numero
        a 16 bit) / htonl (l: long, numero a 32 bit), hton implementato con una macro
        PER TRASFERIRE I DATI SULLA RETE BISOGNA INVOCARE hton
    */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    /*
        dopo aver settato i campi della struct posso invocare la bind (mi metto in ascolto sulla porta che ho indicato)
        se valore di ritorno < 0 ho avuto un problema e stampo un messaggio di errore
    */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    /*
        con la listen dimensiono la coda di ascolto (massimo 5 richieste di connessione accodate in questo caso)
    */
    listen(sockfd,5);
    /*
        inizializzo clilen essendo un parametro che devo rendere scrivibile
    */
    clilen = sizeof(cli_addr);
    /*
        la accept ritorna il nuovo socket (quello su cui leggere e scrivere), se < 0 ho un errore
    */
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    /*
        se arrivo qui newsockfd diventa l'estremo della mia connessione con il client
        leggo e scrivo utilizzando un buffer che inizializzo a 0 (in questo modo qualsiasi cosa scriva dopo sono sicuro che sia null terminated, quindi una stringa standard
        dal punto di vista unix)
    */
    bzero(buffer,256);
    /*
        implemento la logica di protocollo applicativo
        prendo una stringa dal client, la stampo su stdout e dopo mando un messaggio
        eseguo la read sul socket descriptor ritornato dalla accept (leggo al massimo 255 caratteri, in questo modo ho la garanzia che ci sia sempre almeno un byte a 0 alla
        fine del buffer)
    */
    n = read(newsockfd,buffer,255);
    /*
        se read ritorna un valore < 0 stampo un messaggio di errore
    */
    if (n < 0) 
        error("ERROR reading from socket");
    /*
        stampo il messaggio con printf, uso %s dato che so che ho una stringa unix-safe
    */
    printf("Here is the message: %s\n",buffer);
    /*
        invio messaggio di acknowledgement al client, 18 = lunghezza della stringa
    */
    n = write(newsockfd,"I got your message",18);
    /*
        se write ritorna un valore < 0 stampo un messaggio di errore
    */
    if (n < 0) 
        error("ERROR writing to socket");
    return 0; 
}