#include "libCesar.h"
#include <unistd.h>
#include <time.h>
int pipe1[2];
int pipe2[2];
int pipe3[3];

void hijo1() {
    if(fork() == 0) {;
        char readbuffer[256];
        printf("[HIJO1]\tIniciado!\n");
        printf("[HIJO1]\tIntroduzca el comando: ");
        gets(readbuffer);
        printf("[HIJO1]\tEnviando a padre: %s\n", readbuffer);
        write(pipe1[1], readbuffer, strlen(readbuffer)+1);
    }
}

void hijo2() {
    if(fork() == 0) {
        char* encdata;
        char buffer[256];
        int size;
        printf("[HIJO2]\tIniciado y esperando a padre!\n");
        read(pipe2[0], buffer, 256);
        printf("[HIJO2]\tRecibido string de padre: %s\n", buffer);
        //La libreria no codifica el ultimo caracter, sospechamos que debido a que esta pensada con el proposito de que este sea un \n.
        //Por esto, vamos a poner un retorno de carro al final de la palabra y eliminarlo posteriormente.
        size = strlen(buffer);
        buffer[size] = '\n';
        buffer[size+1] = 0;
        if(strncmp(buffer, "cod", 3)) {
            encdata = codifica(buffer+4);
        }else{
            encdata = decodifica(buffer+4);
        }
        if(encdata == NULL) {
            buffer[0] = 0;
            write(pipe3[1], buffer, 256);
            printf("[HIJO2]\tError en libCesar. Devolviendo mensaje de error al padre.\n");
        }else{ 
            buffer[size] = 0;
            write(pipe3[1], encdata, 256);
            printf("[HIJO2]\tDevuelto a padre: %s.\n", encdata);
        }	
    }
}

void padre() {
    printf("[PADRE]\tIniciado!\n");
    char buffer[256];
    char buffer_2[256];
    char time_buffer[80];
    char de[3];
    time_t raw_time;
    struct tm *tm_info;

    FILE *file;

    file = fopen("registro.txt","a+");
    while(1) {
        hijo1();
        read(pipe1[0], buffer, 256);
        printf("[PADRE]\tPadre recibe: %s\n", buffer);
        //Comprobamos si los primeros 3 caracteres son "reg". De ser asi, mostramos el log.
        if(strncmp(buffer, "reg", 3) == 0) {
            printf("[PADRE]\tMostrando registro..\n");
            system("cat registro.txt");
        }else if((strncmp(buffer, "cod ", 4) == 0) || (strncmp(buffer, "dec ", 4) == 0)) {
            printf("[PADRE]\tPadre envia a HIJO2..\n");
            write(pipe2[1], buffer, 256);
            hijo2();
            printf("[PADRE]\tPadre espera a recibir de HIJO2..\n");
            read(pipe3[0], buffer_2, 256);
            if(buffer_2[0] == 0) { 
                printf("[PADRE]\tError al codificar!\n");
            }else{
                printf("[PADRE]\tPadre recibe de HIJO2: %s\n", buffer_2);
                //Codificamos el registro para el log.
                if(strncmp(buffer, "dec", 3) == 0) {strcpy(de, "de"); }else{strcpy(de, ""); }
                time(&raw_time);
                tm_info = localtime(&raw_time);
                strftime(time_buffer, 80, "%d/%m/%y %H:%M:%S", tm_info);
                fprintf(file, "Nueva entrada a las %s\n  Texto a %scodificar: %s\n  Texto %scodificado: %s\n", time_buffer, de, buffer+4, de, buffer_2);
                fflush(file);

            }}
        else{
            printf("[PADRE]\tComando incorrecto!\n");
        }
    }
}
int main(int argc, char const *argv[])
{
    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3);
    padre();	
    return 0;
}


