#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

/*--------------------------------------------------------------
Realiza una codificación o decoficación César.
Las dos funciones recben una cadena de caracteres y devuelven
un puntero a la cadena de caracteres codificada (o decodificada)

Para el cifrado César sólo se utilizan los caracteres de la 'a' 
la 'z' y de la 'A' a la 'Z'. No se incluye 'ñ' ni otros caracteres. 
En caso de que la cadena los incluya las funciones devolverán NULL
----------------------------------------------------------------*/
char* decodifica(char cadena[]);
char* codifica(char cadena[]);

