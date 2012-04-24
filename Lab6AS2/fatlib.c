#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fatlib.h"

/* En la FAT, los clusteres se almacenan empezando en 1.
Con estas macros, convertimos más "legiblemente" de FAT a cero-indexados (posición real en memoria)
*/
#define FAT(x) x+1
#define MEM(x) x-1

/* Variable que almacena el tamaño del cluster */
int fs_cluster_size;
int fat_clusters;

/*************************************************************
 **********   Implementación de funciones  ******************
 **********       definidas en fatlib.h       ******************
 *************************************************************/

 void dump_fs(char* fs, char* name) {
 	FILE *file = fopen(name, "wb");
 	fwrite(fs, CLUSTER_NUMBER, CLUSTER_SIZE, file);
 	fclose(file);
 }

 int* init_fat() {
	//Inicializamos un array de ints de tamaÃ±o 2 * CLUSTER_NUMBER para hacer sitio a
	//el caso de clusters de 512.
 	int *fat = malloc(2*CLUSTER_NUMBER*sizeof(int));
	//pero como por defecto el cluster es de 1024, los clusters van a ser CLUSTER_NUMBER
 	fat_clusters = CLUSTER_NUMBER;
 	int i = 0;
 	for (i = 0; i < CLUSTER_NUMBER; i++) {
 		fat[i] = EMPTY_CLUSTER;
 	}
 	return fat;
 }

 char* init_fs() {
 	fs_cluster_size = CLUSTER_SIZE;
 	char *fs = calloc(CLUSTER_NUMBER, CLUSTER_SIZE);
 	return fs;
 }

 DirEntry* init_directory_table() {
 	DirEntry* dirs = (DirEntry *) malloc(sizeof(DirEntry)*MAX_FILES);
 	int i;
 	for (i = 0; i < MAX_FILES; i++) {
 		strcpy(dirs[i].filename, "");
 		dirs[i].first_cluster = 0;
 	}
 	return dirs;
 }
 void process(char* fs,
 	int* fat,
 	DirEntry* files,
 	char* action, 
 	char* op1, 
 	char* op2) {
 	if(strcmp("add", action) == 0) {
 		printf("Añadiendo archivo %s desde %s\n", op1, op2);
        //Abrimos el archivo
 		FILE *file = fopen(op2, "rb");
        //Obtenemos el tamaño en bytes atravesandolo hasta el final
 		fseek(file, 0L, SEEK_END);
 		int sz = ftell(file);
 		fseek(file, 0L, SEEK_SET);
		//Obtenemos el número de clústeres libres de la FAT, asi como el primero libre
 		int c = 0; int j; int first_free_cluster = -1;
 		for (j = 0; j < fat_clusters; ++j)
 		{
 			if(first_free_cluster == -1 && fat[j] == EMPTY_CLUSTER) first_free_cluster = j;
 			if(fat[j] == EMPTY_CLUSTER) c++;
 		}
		//Obtenemos el tamaño del archivo en clústeres
 		int cluster_file_size = sz / fs_cluster_size;
 		if(sz%fs_cluster_size != 0) cluster_file_size++;
 		printf("El archivo ocupará: %d clusteres, de %d libres, empezando en %d.\n", sz, c, FAT(first_free_cluster));
 		if(cluster_file_size > c) {
 			printf("ERROR: No hay espacio suficiente para almacenar el archivo %s\n", op1);
 			return;
 		}
		//Buscamos el primer DirEntry libre
 		int dir_entry_to_use = -1; int i;
 		for (i = 0; i < MAX_FILES; i++)
 		{
 			if(dir_entry_to_use == -1 && files[i].first_cluster == 0) dir_entry_to_use = i;
 		}
 		if(dir_entry_to_use == -1) {
 			printf("ERROR: Numero maximo de archivos superado.");
 			return;
 		}

 		strcpy(files[dir_entry_to_use].filename, op1);
 		files[dir_entry_to_use].first_cluster = FAT(first_free_cluster);
		//Atravesamos de nuevo, vamos a ir añadiendo clusteres
 		int next_free_cluster = first_free_cluster; int k;
 		int last_cluster;
 		for(j=0; j<cluster_file_size; ++j) {
			//Vaciamos el cluster entero.
 			memset(fs + (CLUSTER_SIZE * next_free_cluster), 0, fs_cluster_size);
			//Copiamos CLUSTER_SIZE datos a la posición dada por:
			// puntero fs + (CLUSTER_SIZE * num cluster)
 			fread(fs + (CLUSTER_SIZE * next_free_cluster), 1, fs_cluster_size, file);
			//Buscamos el siguiente cluster libre
 			first_free_cluster = -1;
 			for (k = next_free_cluster+1; k < fat_clusters; ++k){
 				if(first_free_cluster == -1 && fat[k] == EMPTY_CLUSTER) first_free_cluster = k;
 			}
			//Guardamos en la FAT que el siguiente trozo de archivo irá ahí
 			fat[next_free_cluster] = FAT(first_free_cluster);
 			last_cluster = next_free_cluster;
 			next_free_cluster = first_free_cluster;
 		}
 		fat[last_cluster] = FINAL_CLUSTER;
 	}
 	if(strcmp("del", action) == 0) {
 		printf("Borrando archivo %s\n", op1);
        //Buscamos el archivo en la DirTable
 		int c; int fi = -1;
 		for(c=0; c<MAX_FILES; c++) { if(strcmp(op1, files[c].filename) == 0) { fi = c; break; } }
 			if(fi == -1) {
 				printf("ERROR: Archivo %s no encontrado.\n", op1);
 			}
 			int cur_cluster = files[fi].first_cluster;
 			int next_cluster;
 			do {
 				next_cluster = fat[MEM(cur_cluster)];
 				fat[MEM(cur_cluster)] = EMPTY_CLUSTER;
 				cur_cluster = next_cluster;
 			} while(next_cluster != FINAL_CLUSTER); 
 		}
 		if(strcmp("format", action) == 0) {
 			printf("Formateando sistema de archivos!\n");
 			int i;
 			if(strcmp(op1, "c") == 0) {
 				printf("Formateo completo, poniendo FS en blanco..\n");
 				for (i = 0; i < CLUSTER_NUMBER*CLUSTER_SIZE; ++i){ fs[i] = 0;	}
 					if(strcmp("1024", op2) == 0){
 						printf("Estableciendo numero de clusters a 1024..\n");
 						fat_clusters = 200;
 						fs_cluster_size = 1024;
 					}
 					if(strcmp("512", op2) == 0){
 						printf("Estableciendo numero de clusters a 512..\n");
 						fat_clusters = 400;
 						fs_cluster_size = 512;
 					}
 				}
 				printf("Formateando FAT..\n");

 				for (i = 0; i < fat_clusters; ++i){
 					fat[fat_clusters] = EMPTY_CLUSTER;
 				}
 				printf("Borrando lista de archivos..\n");	
 				for (i = 0; i < MAX_FILES; ++i)
 				{
 					strcpy(files[i].filename, "");
 					files[i].first_cluster = 0; 
 				}
 			}


 		}

/*************************************************************
 ********** Prototipos de tus funciones       ****************
 *************************************************************/

// Comentad el código


/*************************************************************
 ********** Implementación de la practica *********************
 *************************************************************/

/*** Se proporciona implementada la función que vuelca la fat por pantalla ***/ 
 void dump_fat(int* fat) {
 	int i;
 	printf("[ ");
 		for (i=0; i < fat_clusters; i++) {
 			printf("%3d:%3d  \t", i+1, *(fat+i));
 		}
 		printf("]");
 		fflush(NULL);
 	}

