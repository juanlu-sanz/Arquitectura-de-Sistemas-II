
#define MAX_RENOS 9
#define MAX_ELFOS 3

#include <pthread.h>
#include <stdio.h>

int sala_espera = 0;

int elfos_dudosos = 0;

pthread_mutex_t santa_duerme = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_libre = PTHREAD_COND_INITIALIZER;

pthread_cond_t sala_espera_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t sala_espera_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elfos_dudosos_m = PTHREAD_MUTEX_INITIALIZER;

void *llegada_reno(void *param) {

    while (1) {
#ifndef NOSLEEP
        sleep(1 * random() % 5);
#endif
        pthread_mutex_lock(&santa_duerme);
        pthread_mutex_lock(&sala_espera_m);
        if (sala_espera < MAX_RENOS) {
            sala_espera++;
            printf("Llega reno... Total: %d\n", sala_espera);
            if (sala_espera == MAX_RENOS) {
                //Esta la sala llena con todos los renos?
                pthread_mutex_unlock(&sala_espera_m);
                printf("todos renos, repartimos!\n");
                pthread_cond_signal(&santa_libre);
                pthread_cond_wait(&sala_espera_cond, &santa_duerme);
            } else {
                //no? Desbloqueamos variables
                pthread_mutex_unlock(&sala_espera_m);
                pthread_cond_wait(&sala_espera_cond, &santa_duerme);
            }
        } else {
            pthread_mutex_unlock(&sala_espera_m);
        }
        pthread_mutex_unlock(&santa_duerme);
    }
}

void elfo_duda() {
    while (1) {
#ifndef NOSLEEP
        sleep(1 * random() % 5);
#endif		
        pthread_mutex_lock(&santa_duerme);
        pthread_mutex_lock(&elfos_dudosos_m);
        elfos_dudosos++;
        printf("Llega elfo.. Total: %d\n", elfos_dudosos);
        if (elfos_dudosos >= MAX_ELFOS) {
            pthread_mutex_unlock(&elfos_dudosos_m);
            printf("todos elfos, santa llamado!\n");
            pthread_cond_signal(&santa_libre);
            pthread_cond_wait(&sala_espera_cond, &santa_duerme);
        } else {
            pthread_mutex_unlock(&elfos_dudosos_m);
            pthread_cond_wait(&sala_espera_cond, &santa_duerme);
        }

        pthread_mutex_unlock(&santa_duerme);
    }
}

void repartir_regalos() {
    pthread_mutex_lock(&sala_espera_m);
    sala_espera = 0;
    pthread_mutex_unlock(&sala_espera_m);
    printf("repartiendo regalos. Ahora hay %d renos\n", sala_espera);
#ifndef NOSLEEP
    sleep(1 * random() % 5);
#endif
    printf("regalos repartidos!\n");
    pthread_cond_broadcast(&sala_espera_cond);
}

void resolver_dudas() {
    pthread_mutex_lock(&elfos_dudosos_m);
    elfos_dudosos = elfos_dudosos - 3;

    printf("resolviendo dudas, ahora hay %d elfos\n", elfos_dudosos);
    pthread_mutex_unlock(&elfos_dudosos_m);

#ifndef NOSLEEP
    sleep(1 * random() % 5);
#endif
    printf("dudas contestadas!\n");
    pthread_cond_broadcast(&sala_espera_cond);
}

void santa() {

    while (1) {

        pthread_mutex_lock(&santa_duerme);
        printf("santa duerme\n");

        while ((sala_espera < MAX_RENOS) && (elfos_dudosos < MAX_ELFOS)) {
            pthread_cond_wait(&santa_libre, &santa_duerme);
        }
        printf("santa se despierta!\n");
        pthread_mutex_lock(&elfos_dudosos_m);
        pthread_mutex_lock(&sala_espera_m);
        if (sala_espera >= MAX_RENOS) {
            pthread_mutex_unlock(&elfos_dudosos_m);
            pthread_mutex_unlock(&sala_espera_m);
            repartir_regalos();
        } else if (elfos_dudosos >= MAX_ELFOS) {
            pthread_mutex_unlock(&elfos_dudosos_m);
            pthread_mutex_unlock(&sala_espera_m);
            resolver_dudas();
        }
        pthread_mutex_unlock(&santa_duerme);
    }
}

int main() {

    int i;
    pthread_t renos[MAX_RENOS];
    pthread_t elfos[MAX_ELFOS];
    pthread_t santa_t;

    pthread_create(&santa_t, NULL, (void *) &santa, NULL);


    for (i = 0; i < MAX_RENOS; i++) {
        pthread_create(&renos[i], NULL, (void *) &llegada_reno, NULL);
    }

    for (i = 0; i < MAX_ELFOS; i++) {
        pthread_create(&elfos[i], NULL, (void *) &elfo_duda, NULL);
    }

    for (i = 0; i < MAX_RENOS; i++) {
        pthread_join(renos[i], NULL);
    }
    for (i = 0; i < MAX_ELFOS; i++) {
        pthread_join(elfos[i], NULL);
    }

    pthread_join(santa_t, NULL);

    pthread_mutex_destroy(&santa_duerme);

    return 0;
}
