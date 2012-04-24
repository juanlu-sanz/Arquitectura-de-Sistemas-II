#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queues.h"

char * this_queue_name = "";


int main(int argc, char const *argv[]) {
	printf("----- Client 2 Running -----\n");
	mqd_t client = init_queue("/mailbox2", O_RDONLY);
	mqd_t mta = init_queue("/mta", O_WRONLY);
	while(1) {
		printf("Sending message to 1...\n");
		msg message;

		strcpy(message.data, "Hola desde cliente2!");
		message.from = 2;
		message.to = 1;

		write_queue(mta, message);

		printf("Sent. Waiting for message..\n");
		msg recvmsg = read_queue(client);
		printf("Message received from: %i to: %i:\n%s\n\n", recvmsg.from, recvmsg.to, recvmsg.data);
	}
	return 0;
}