#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>


#define MAX_DATA_SIZE 256
#define MAX_TO_SIZE 64
#define MAX_FROM_SIZE 64
#define TOTAL_MSG_SIZE MAX_DATA_SIZE+4+4
#define STANDARD_PRORITY 1



typedef struct {
	char data[150];
	int to;
	int from;
} msg ;


void print_message(msg mesg) {
	printf("Message: %s from: %i to: %i\n", mesg.data, mesg.from, mesg.to);
}

mqd_t init_queue (char* queue_name,int open_flags) {
	struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = sizeof(msg);
    attr.mq_curmsgs = 0;
	mqd_t queue = mq_open(queue_name, open_flags, 0644, &attr);
	return queue;
};

static void write_queue (mqd_t mq_desc, msg data) {
	/* Sending... */
	int send_result = mq_send(mq_desc,(char*) &data, sizeof(msg), STANDARD_PRORITY);

	/* Did it work? */
	if (send_result == 1){
		printf("[QUEUE] Message sent correcly.\n");
	} else {
		printf("There has been an error while sending: %s.\n", strerror(errno));
	}
}

static msg read_queue (mqd_t mq_desc) {
	msg message; unsigned int prio;
	int result = mq_receive(mq_desc, (char *)&message, sizeof(msg), &prio);
	if (result != -1){
		printf("[QUEUE] Message read from queue.\n");
	} else {
		printf("There has been an error while reading: %s.\n", strerror(errno));
	}
	return message;
}

static int close_queue (mqd_t mq_desc) {
	return mq_close(mq_desc);
}
static int unlink_queue (char* queue_name) {
	return mq_unlink(queue_name);
};