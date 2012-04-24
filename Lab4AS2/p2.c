#include <pthread.h>
#include <stdio.h>

pthread_mutex_t escribiendo_letras = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t letra_opuesta_escrita = PTHREAD_COND_INITIALIZER;
int first = 0;

void escribirA() {
    while(1) {
        printf("bloqueando mutex escribirA!\n");

        int c;

        printf("bloqueado mutex escribirA!\n");

        for(c=0; c<5; c++) {
            pthread_mutex_lock(&escribiendo_letras);
            printf("A\n");
            pthread_cond_signal(&letra_opuesta_escrita);
            pthread_cond_wait(&letra_opuesta_escrita, &escribiendo_letras);
            sleep(random()%2);
            pthread_mutex_unlock(&escribiendo_letras);
            printf("acaba a\n");
        }
    }
}

void escribirB() {
    while(1) {
        printf("bloqueando mutex escribirB!\n");
        int c;

        printf("bloqueado mutex escribirB!\n");

        for(c=0; c<5; c++) {
            pthread_mutex_lock(&escribiendo_letras);
            printf("B\n");
            pthread_cond_signal(&letra_opuesta_escrita);
            pthread_cond_wait(&letra_opuesta_escrita, &escribiendo_letras);
            sleep(random()%2);
            pthread_mutex_unlock(&escribiendo_letras);	
            printf("acaba b\n");
        }

    }
}

int main(int argc, char const *argv[])
{
    pthread_t hilo1;
    pthread_t hilo2;

    pthread_create(&hilo1, NULL, (void *) escribirA, NULL);
    pthread_create(&hilo2, NULL, (void *) escribirB, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}