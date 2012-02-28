#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMBER_OF_ADDITIONS 5
#define CASH_TO_ADD 10

#define NUMBER_OF_SUBTRACTIONS 5
#define CASH_TO_SUBTRACT 7

pthread_mutex_t account_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_mutex = PTHREAD_COND_INITIALIZER;

void add_money (int *account) {
	int i = 0;
	for (i = 0; i < NUMBER_OF_ADDITIONS; i++) {
		pthread_mutex_lock(&(account_mutex));
		(*account) += CASH_TO_ADD;
		printf("[Add %d]		We just added %dUSD! Now you have %dUSD!\n", i + 1, CASH_TO_ADD, *account);
		pthread_cond_broadcast(&(condition_mutex));
		//	printf("Broadcasted\n");
		pthread_mutex_unlock(&(account_mutex));
	}
}

void withdraw_money(int *account) {
	int i;
	for (i = 0; i < NUMBER_OF_SUBTRACTIONS; i++) {
		pthread_mutex_lock(&(account_mutex));
		while ((*account)-CASH_TO_SUBTRACT < 0) {
			printf("[Remove]	Ops! You seem to have no cash!\n");
			pthread_cond_wait(&condition_mutex, &account_mutex);
			printf("[Remove]	Someone Added something! let's see if it's enough\n");
		}
		(*account) -= CASH_TO_SUBTRACT;
		printf("[Remove %d]	We just withdrew %dUSD! You still have %dUSD!\n", i + 1, CASH_TO_SUBTRACT, (*account));
		pthread_mutex_unlock(&(account_mutex));
	}
}

int main(int argc, const char *argv[]) {
	int account = 0;
	printf("Welcome! you have an initial amount of %dUSD!\n", account);
	pthread_t adder, withdrawer;

	pthread_create(&withdrawer, NULL, (void *) withdraw_money, (void *) &account);
	pthread_create(&adder, NULL, (void *) add_money, (void *) &account);

	pthread_join(withdrawer, NULL);
	pthread_join(adder, NULL);
	printf("Done! Thank you for using our service!\n");
	return 0;
}
