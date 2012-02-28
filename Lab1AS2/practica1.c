#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_SIZE 1003

struct proceso_struct {
	struct proceso_struct *prev;
	char id[MAX_ID_SIZE];
	int tiempo_restante;
	struct proceso_struct *next;
};

typedef struct proceso_struct *proceso;

proceso Nueva_Lista(){
	return NULL;
}

proceso Nuevo_Nodo (char *id, int tiempo_restante){
	proceso new = malloc(sizeof(struct proceso_struct));
	strcpy(new->id, id);
	new->tiempo_restante = tiempo_restante;
	return new;
}

proceso Insertar_Nodo(proceso lista, proceso aInsertar){
	if (lista == NULL){
		lista = aInsertar;
		lista->next = lista;
		lista->prev = lista;
		return lista;
	} else {
		proceso listaTest = lista;
		do {
			if (strcmp(listaTest->id, aInsertar->id) == 0) {
				listaTest->tiempo_restante += aInsertar->tiempo_restante;
				printf("[Actualizando]	Proceso %s tiene ahora %d ciclo\n", listaTest->id, listaTest->tiempo_restante);
				return lista;
			}
			listaTest = listaTest->next;
		} while (lista != listaTest);
		printf("[Añadido]	Proceso	%s con %d ciclos\n", aInsertar->id, aInsertar->tiempo_restante);
		proceso previous = lista->prev;
		previous->next = aInsertar;
		aInsertar->prev = previous;
		aInsertar->next = lista;
		lista->prev = aInsertar;
	}
	return lista;
}

proceso Insercion_Nodo(proceso lista, char *id, int tiempo_restante) {
	proceso temp = Nuevo_Nodo(id, tiempo_restante);
	lista = Insertar_Nodo(lista, temp);
	return lista; 
}

proceso Elimino_Nodo (proceso nodo) {
	printf("[Borrado]	Proceso %s ha sido borrado\n", nodo->id);
	free(nodo);
	return NULL;
}

proceso Elimino_Lista (proceso lista) {
	if (lista == NULL){
		printf("La lista ya esta vacia, nada que hacer\n");
		return NULL;
	}

	while(lista->next != lista) {
		proceso aBorrar = lista->next;
		lista->next = aBorrar->next;
		Elimino_Nodo(aBorrar);
	}
	Elimino_Nodo(lista);
	return NULL;
}

proceso Aplicar_Quanto(proceso lista, int numeroCiclos) {
	if (lista == NULL) {
		printf("No quedan procesos!\n");
		return NULL;
	}
	lista->tiempo_restante -= numeroCiclos;
	printf("[Quanto]	Proceso %s ejecuta %d ciclos ", lista->id, numeroCiclos);
	if (lista->tiempo_restante <=0) {
		printf("y termina");
		if (lista->next == lista) {
			Elimino_Nodo(lista);
			return NULL;
		}
		lista->prev->next = lista->next;
		lista->next->prev = lista->prev;
		proceso tmp = lista->next;
		Elimino_Nodo(lista);
		lista = tmp;
	} else {
		printf("y le quedan %d ciclos\n", lista->tiempo_restante);
		lista = lista->next;
	}
	return lista;
}

/*
 * Lee una linea y devuelve un proceso 
 */
proceso parseLinea(proceso lista, char* linea) {
    char id[MAX_ID_SIZE + 1];
    int tiempo_restante;
    char format[20];
    sprintf(format, "%%%ds %%d\n", MAX_ID_SIZE);
    sscanf(linea, format, id, &tiempo_restante);
    proceso nuevo = Insercion_Tiempo(lista, id, tiempo_restante);
    return nuevo;
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        /* En caso de que no haya los argumentos correctos */
        printf("Error parseando argumentos.\nEl uso correcto seria: \n%s [Tamaño del Quanto] [Numero de Quantos] [Fichero de entrada]\n", argv[0]);
        return 0;
    }
    proceso lista = Nueva_Lista();
    int q_size = atoi(argv[1]);
    int q_count = atoi(argv[2]);
    FILE *file = fopen(argv[3], "r");
    char read[MAX_ID_SIZE + 11];
    while (!feof(file)) {
        fgets(read, MAX_ID_SIZE + 11, file);
        if (strlen(read) + 1 >= MAX_ID_SIZE) {
            printf("[ERROR] El ID del proceso es demasiado largo o el número de ciclos es demasiado alto.\n");
            return 0;
        }
        lista = parseLinea(lista, read);
		}
		int q_count_done;
    for (q_count_done = 0; q_count_done < q_count; q_count_done++) {
        lista = Aplicar_Quanto(lista, q_size);
    }
    /* Cerramos y limpiamos todo */
    Elimino_Lista(lista);
    fclose(file);

    return 0;
}

































