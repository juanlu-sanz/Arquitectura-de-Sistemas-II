#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queues.h"

char * this_queue_name = "";


int main(int argc, char const *argv[]) {
	printf("----- Client 1 Running -----\n");
	mqd_t client = init_queue("/mailbox1", O_RDONLY);
	mqd_t mta = init_queue("/mta", O_WRONLY);
	while(1) {
		printf("Sending message to 2...\n");
		msg message;

		strcpy(message.data, "Hola desde cliente1!");
		message.from = 1;
		message.to = 2;

		write_queue(mta, message);

		printf("Sent. Waiting for message..\n");
		msg recvmsg = read_queue(client);
		printf("Message received from: %i to: %i:\n%s\n\n", recvmsg.from, recvmsg.to, recvmsg.data);
	}
	return 0;
}