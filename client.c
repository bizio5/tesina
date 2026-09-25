#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define port 9998
#define buffersize 1024


int main(int argc, char **argv){
    
    //printf("ciao client\n");

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    int client_fd;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }
    //converte il binario in questo caso l indirzzo è per la stessa macchina
    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0) {
        perror("Errore nella conversione dell'indirizzo IP");
        exit(EXIT_FAILURE);
    } 


    if (connect(client_fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Errore nella connessione al server");
        exit(EXIT_FAILURE);
    }

    char messaggio[buffersize] = "Ciao server, sono il client!";
    send(client_fd, messaggio, strlen(messaggio), 0);

    close(client_fd);



    
}