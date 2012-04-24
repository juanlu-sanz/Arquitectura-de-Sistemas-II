#include "constants.h"

/*************************************************************
 ******************* FATlib datatypes ************************
 *************************************************************/

typedef struct DirEntry {
    char filename[MAX_FILES_NAME_LENGTH];
    int first_cluster;
} DirEntry;

/* Variable que almacena el tamaño del cluster */
int fat_cluster_size;
/*************************************************************
 ********************* FATlib API ****************************
 *************************************************************/

/**
 * void dump_fs(char* fs, char* name);
 * Muestra el contenido del sistema de ficheros simulado por pantalla
 * @param fs        Sistema de ficheros simulados FAT
 * @param name      Fichero de volcado
 *
 * @return void
 */
void dump_fs(char* fs, char* name);

/**
 *  void dump_fat(int* fat);
 *  Muestra el contenido de la FAT por pantalla
 *
 * @param FAT
 */
void dump_fat(int* fat);

/**
 * int* init_fat();
 * Inicializa la FAT, reservando memoria y completándola con ceros. 
 *
 * @return puntero a la FAT
 */
int* init_fat();

/**
 * DirEntry* init_directory_table(); 
 * Inicializa la tabla de directorio, estableciendo los punteros a los cluster a cero 
 * y los nombres de los ficheros a un string vacío
 *
 * @return puntero a la tabla de directorio
 */
DirEntry* init_directory_table();

/** 
 * char* init_fs()
 * Inicializa el sistema de ficheros simulado con todos los cluster a cero 
 *
 * @return puntero al sistema de ficheros
 */
char* init_fs();

/**
 * Procesa las acciones indicas en el array action en el sistema ficheros
 * 
 * @param fs        Sistema de ficheros simulado
 * @param fat       FAT simulada
 * @param files     Tabla de directorio
 * @param action    Acción a ejecutar (add, del, format)
 * @param op1       Nombre del fichero en el sistema simulado o para el caso de un formateo el tipo de formateado c "completo" o f "fast (rápido)"
 * @param op2       Nombre del fichero en el sistema real o para el caso de un formateo el nuevo tamaño de cluster (512 o 1024)
 *
 * @return void
 */
void process(char* fs,
             int* fat,
             DirEntry* files,
             char* action, 
             char* op1, 
             char* op2);
