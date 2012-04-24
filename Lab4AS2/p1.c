#include <pthread.h>
#include <stdio.h>

/*Creamos un cerrojo, cuando alguien este escribiendo, nadie mas podra*/
pthread_mutex_t escribiendo_letras = PTHREAD_MUTEX_INITIALIZER;

/*
 * Esta funcion escribe 
 */

void escribirA() {
    while (1) {
        int c;
        pthread_mutex_lock(&escribiendo_letras);

        for (c = 0; c < 5; c++) {
            printf("A\n");
            sleep(random() % 2);
        }
        pthread_mutex_unlock(&escribiendo_letras);
    }
}

void escribirB() {
    while (1) {
        int c;
        pthread_mutex_lock(&escribiendo_letras);

        for (c = 0; c < 5; c++) {
            printf("B\n");
            sleep(random() % 2);
        }
        pthread_mutex_unlock(&escribiendo_letras);
    }
}

int main(int argc, char const *argv[]) {
    pthread_t hilo1;
    pthread_t hilo2;

    pthread_create(&hilo1, NULL, (void *) escribirA, NULL);
    pthread_create(&hilo2, NULL, (void *) escribirB, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}