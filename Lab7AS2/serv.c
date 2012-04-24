#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include "queues.h"


int main(int argc, char const *argv[])
{
	printf("----- Server running.. -----\n");

	unlink_queue("/mta");
	unlink_queue("/mailbox1");
	unlink_queue("/mailbox2");
	//We create a queue for MTA input
	mqd_t input = init_queue("/mta", O_CREAT | O_RDONLY);
	//and two more for writing to clients
	mqd_t client1 = init_queue("/mailbox1", O_CREAT | O_WRONLY);
	mqd_t client2 = init_queue("/mailbox2", O_CREAT | O_WRONLY);

	//loop
	msg message;
	while(1) {
		printf("Waiting for messages..\n");
		message = read_queue(input);
		printf("Transmitting message \"%s\" from: %i to: %i\n", message.data, message.from, message.to);
		if(message.to == 1) {
			printf("Sending message to client1\n");
			write_queue(client1, message);
		}
		if(message.to == 2){
			printf("Sending message to client2\n");
			write_queue(client2, message);
		}
	}
	return 0;
}