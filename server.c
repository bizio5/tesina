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

int connessione = 0;

struct param{
    int socket;
    char *socket_name;
};

struct messaggio{
        char destinatario[32];
        char oggetto[64];
        char testo[1024];
};

void *func_thread(void *arg){

    struct param * param_thread = (struct param *) arg;
    int fd = open(param_thread -> socket_name,O_CREAT | O_RDWR ,0666);

    while(1){
        int scelta;
        printf("thread %d\n",connessione);
        ssize_t byte = recv(param_thread -> socket,&scelta,sizeof(int),0);

        if (byte <= 0) {
            printf("Client disconnesso o errore.\n");
            break; // ESCE DAL CICLO: Evita il loop infinito!
        }

        switch(scelta) {
            //valutare se fare funzioni per i vari case
            case 1:
                printf("Richiesta lettura messaggi...\n");
                break;
                
            case 2: {
                printf("Ricezione nuovo messaggio in corso...\n");
                
                struct messaggio msg_ricevuto;
                ssize_t byte_msg = recv(param_thread->socket, &msg_ricevuto, sizeof(struct messaggio), 0);
                
                if (byte_msg > 0) {
                    printf("Destinatario: %s", msg_ricevuto.destinatario);
                    printf("Oggetto: %s", msg_ricevuto.oggetto);
                    printf("Testo: %s", msg_ricevuto.testo);
                }
                break;
            }
            
            case 3:
                printf("Richiesta cancellazione...\n");
                break;
                
            case 4:
                printf("Il client ha chiesto di chiudere.\n");
                close(param_thread->socket);
                break;
                
            default:
                printf("Comando non valido ricevuto.\n");
                break;
        }
        if (scelta == 4) {
            break;
        }
       
    }
    close(fd);
    close(param_thread->socket);
    free(param_thread);
    return NULL;

}

int main(int argc, char **argv){
    
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    //htons serve per convertire il numero di porta in network byte order
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Errore nella setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Errore nel bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
            perror("Errore nella listen");
            exit(EXIT_FAILURE);
        }
    
    while(1){

        struct sockaddr_in clientAddr;
        socklen_t addrlen = sizeof(clientAddr);
        pthread_t thread;
        struct param *param_main = malloc(sizeof(struct param));


        if ( (param_main -> socket = accept(server_fd, (struct sockaddr *)&clientAddr, &addrlen)) < 0) {
            perror("Errore nell'accept");
            exit(EXIT_FAILURE);
        }
        else{
            connessione++;
            printf("Numero di connessioni: %d\n", connessione);

            param_main -> socket_name = inet_ntoa(clientAddr.sin_addr);

            pthread_create(&thread,NULL,func_thread, param_main);
        }
    }
}