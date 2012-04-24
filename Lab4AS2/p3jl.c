#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int buffer[10];
int current_position = 0;
int total_generated = 0;

pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t go_calculate = PTHREAD_COND_INITIALIZER;
pthread_cond_t go_generate = PTHREAD_COND_INITIALIZER; 

void printArray(int buffer[]) {
	int i;
	for (i = 0; i < current_position; i++) {
		printf("[%d] ", buffer[i]);
	}
	printf("\n");
}

void generateNumbers(){
	int random_number, cpos;
	int random_seed = time(NULL);
	srand(random_seed);
	current_position = 0;
	while (total_generated < 20) {
		random_number = rand() % 10;

		pthread_mutex_lock(&(buffer_lock));					/*LOCK*/
		printf("[PRINTING]	The Array is:	");
		buffer[current_position] = random_number;
		current_position++;

		printArray(buffer);
		cpos = current_position;
		total_generated++;

		if (random_number == 4 || cpos == 10 || total_generated == 20) {
			printf("=====[CONDITION MET]=====\n");
			pthread_cond_broadcast(&(go_calculate));
			if (total_generated != 20) {
				pthread_cond_wait(&(go_generate), &(buffer_lock));
			}
		}
		pthread_mutex_unlock(&(buffer_lock));				/*UNLOCK*/		
	}
	printf("Done! We got 20 numbers!\n");
}

void calculateResult (){
	int sum = 0;
	while (1) {
		/* Waiting for a call */
		printf("[WAITING]\n");
		//		printf("[CALCULATING]\n");

		pthread_mutex_lock(&(buffer_lock));					/*LOCK*/
		int i;
		pthread_cond_wait(&(go_calculate), &(buffer_lock));
		for (i = 0; i < current_position; i++) {
			sum += buffer[i];
			buffer[i] = 0;
		}
		printf("[Result]	The current sum is %d\n", sum);
		current_position = 0;
		if (total_generated == 20) {
			printf("[DONE]\n");
			pthread_cond_broadcast(&(go_generate));
			return;
		}
		pthread_cond_broadcast(&(go_generate));
		pthread_mutex_unlock(&(buffer_lock));				/*UNLOCK*/
		printf("DONE 2\n");
	}
	return;
}

int main(int argc, const char *argv[]) {
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, (void *) generateNumbers, NULL);
	pthread_create(&thread2, NULL, (void *) calculateResult, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
