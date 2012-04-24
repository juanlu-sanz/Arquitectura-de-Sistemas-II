#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main () {
	printf ("Inicio del proceso padre. PID=%d\n",getpid ());
	fork();
	int i;
	for (i=0;i<10;i++){
		printf("Soy PID=%d\n", getpid ());
//		sleep(1);
	}
	printf ("Fin del proceso %d\n", getpid ());
	exit (0);
}
