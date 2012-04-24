#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

main () {
	printf ("Ejemplo de fork.\n");
	printf ("Inicio del proceso padre. PID=%d\n",getpid ());
	if (fork() == 0) { 
		printf ("A - Soy PID=%d\n", getpid ());
		sleep (1);
	} else { 
		printf ("B - Soy PID=%d\n", getpid ());
		sleep (1);
	}
	printf ("Fin del proceso %d\n", getpid ());
	exit (0);
}
