/*
 * Fichero definición constantes
 *
 */

/* Número de clusters en el inicio*/
#define CLUSTER_NUMBER 200

/* Tamaño inicial del cluster en el inicio*/
#define CLUSTER_SIZE 1024

/* Tamaño total del sistema ficheros fs = CLUSTER_NUMBER * CLUSTER_SIZE */

/* Símbolo indica final de un cluster */
#define FINAL_CLUSTER 256

/* Simbolo indica cluster vacío */
#define EMPTY_CLUSTER 0

/* Numero máximo de ficheros en el sistema */
#define MAX_FILES 10

/* Longitud del nombre de los ficheros */
#define MAX_FILES_NAME_LENGTH 11  /* 8 + 3 */
