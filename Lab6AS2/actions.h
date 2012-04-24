/**
 * Acciones a realizar
 */

/** Numero total de acciones */
#define NUMBER_OF_ACTIONS 15

/** 
 * Arra y que especifica las acciones a realizar. 
 * Cada tupla nos indica:
 *  - Accion: "add" añade un fichero al sistema de ficheros.
 *            "del" elimina un fichero del sistema de ficheros.
 *  - Nombre: nombre que tendrá el fichero en el sistema de ficheros simulados.
 *  - Realname: nombre real del fichero que se añadirá al sistema simulado.
 */ 
char* actions[10*NUMBER_OF_ACTIONS] = { 
  "add", "file1", "f3.txt", 
  "add", "file2", "f3.txt", 
  "add", "file3", "f3.txt", 
  "add", "file4", "f3.txt", 
  "del", "file2", "", 
  "add", "file5", "f2.txt", 
  "add", "file6", "f3.txt", 
  "del", "file1", "", 
  "add", "file7", "f1.txt", 
  "del", "file4", "",
  "format", "c", "512",
  "add", "file9", "f3.txt",
  "add", "file10", "f1.txt",
  "format", "f", "",
  "add", "file11", "f1.txt"
};
