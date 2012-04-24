#include <pthread.h>
#include <stdio.h>

int buffer[10];
int bpos;
int gen; 
int sum;

char calc;
pthread_mutex_t waiter1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waiter2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bufferAccess = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t mandaCalcular = PTHREAD_COND_INITIALIZER;
pthread_cond_t mandaGenerar = PTHREAD_COND_INITIALIZER;

void generarNumero() {
    pthread_mutex_lock(&waiter1);
    int randi = 0;
    while(1) {
        randi = rand()%10;
        /*printf("He generado el %d! en la pos %d\n", randi, bpos);*/
        pthread_mutex_lock(&bufferAccess);
        buffer[bpos] = randi;
        bpos++; gen++;
        pthread_mutex_unlock(&bufferAccess);
        if(randi == 4 ||bpos == 10 ||gen ==20) {
            printf("--- Llamando al hilo calculador.. ---\n");
            pthread_mutex_lock(&waiter2);
            calc = 1;
            pthread_mutex_unlock(&waiter2);
            pthread_cond_signal(&mandaCalcular);
            if(gen == 20) {
                pthread_mutex_unlock(&waiter1);
                return;
            }
            while(calc!=0) pthread_cond_wait(&mandaGenerar, &waiter1);
        }
    }

}

void calcularResultado()  {
    int i;
    pthread_mutex_lock(&waiter2);
    while(1) {
        printf("Hilo2 esperando para calcular resultado...\n");
        while(calc!=1) { 
            pthread_cond_wait(&mandaCalcular, &waiter2);
        }
        pthread_mutex_lock(&bufferAccess);
        for (i = 0; i < bpos; i++) {
            printf("%d- ", buffer[i]);
            sum = sum + buffer[i];
        }
        printf("\nNumeros generados: %d.\nSuma: %d.\n", gen, sum);
        bpos=0;

        pthread_mutex_unlock(&bufferAccess);
        if(gen >= 20) {
            printf("20 números generados! Fin de ejecución!\n");
            pthread_mutex_unlock(&waiter2);
            return;
        }
        pthread_mutex_lock(&waiter1);
        calc=0;
        pthread_mutex_unlock(&waiter1);
        pthread_cond_signal(&mandaGenerar);
    }

}

int main(int argc, const char *argv[])
{
    bpos = 0;
    gen = 0;
    sum = 0;
    calc = 0;
    pthread_t hilo1;
    pthread_t hilo2;
    srand(time(NULL)); 
    pthread_create(&hilo1, NULL, (void *) calcularResultado, NULL);
    pthread_create(&hilo2, NULL, (void *) generarNumero, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    return 0;
}