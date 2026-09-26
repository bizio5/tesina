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

#define port 9997
#define buffersize 1024

struct messaggio{
        char destinatario[32];
        char oggetto[64];
        char testo[1024];
};

void letturamessaggi(int client_fd,int scelta){

    printf("opzione1\n");
    send(client_fd,&scelta,sizeof(int),0);

}

void spedizione(int client_fd,int scelta ){

    printf("opzione2\n");
    send(client_fd,&scelta,sizeof(int),0);
    
    struct messaggio msg;
    printf("inserisci destinatario:");
    fgets(msg.destinatario,sizeof(msg.destinatario),stdin);

    printf("inserisci oggetto:");
    fgets(msg.oggetto,sizeof(msg.oggetto),stdin);

    printf("inserisci testo:");
    fgets(msg.testo,sizeof(msg.testo),stdin);


    send(client_fd,&msg,sizeof(struct messaggio),0);


}

void cancellazione(int client_fd,int scelta){
    printf("opzione3\n");
    send(client_fd,&scelta,sizeof(int),0);
}









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

     while(1){
        printf("inserire cosa si vuole fare digitando il numero associato all operaizone:\n");
        printf("1)lettura di tutti i messaggi inviati\n");
        printf("2)spedire un messaggio a un utente\n");
        printf("3)cancellazioni di tutti i messaggi ricevuti\n");
        printf("4)chiudere connessione\n");
        
        char richiesta[3];

        fgets(richiesta,sizeof(richiesta),stdin);
        int scelta = atoi(richiesta);

        printf("%d\n",scelta);
        
        switch (scelta) {
            case 1:
                letturamessaggi(client_fd,scelta);
                break;
            case 2:
                spedizione(client_fd,scelta);
                break;
            case 3:
                cancellazione(client_fd,scelta);
                break;
            case 4:
                close(client_fd);
                printf("connesione chiusa\n");
                break;
            default:
                printf("inserire numero valido\n");
        }
        if (scelta == 4)
            break;
    }
}

