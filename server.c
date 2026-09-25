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
    
    printf("ciao server\n");

    int new_socket, server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    //htons serve per convertire il numero di porta in network byte order
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Errore nel bind");
        exit(EXIT_FAILURE);
    }
    while(1){
        if (listen(server_fd, 3) < 0) {
            perror("Errore nella listen");
            exit(EXIT_FAILURE);
        }

        if ( (new_socket = accept(server_fd, (struct sockaddr *)&servAddr, (socklen_t*)&servAddr)) < 0) {
            perror("Errore nell'accept");
            exit(EXIT_FAILURE);
        }
        
        char messaggio[buffersize];
        recv(new_socket, messaggio, buffersize, 0);
        printf("Messaggio ricevuto dal client: %s\n", messaggio);

        close(new_socket);

    }

}