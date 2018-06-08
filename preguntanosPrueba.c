#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("Bienvenido a Preguntanos\n");

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int puntaje = 0;

    fp = fopen("test.txt", "r");
    //levanta el archivo de preguntas
    if (fp == NULL)
        exit(EXIT_FAILURE);

    read = getline(&line, &len, fp);
    while (!feof(fp))
    {
        int correcta = -1;
        int i = 0;
        if (*line == 'P') //si es pregunta
        {
            printf("\nPREGUNTA: %s", (line + 3 * sizeof(char)));
            //no hace nada, la imporime nomas
            while ((read = getline(&line, &len, fp)) != -1 && *line == 'R')
            {
                //ahora mientras lleguen respuestas las muestro
                i++;
                if (*(line + 1 * sizeof(char)) == 'C')
                {
                    correcta = i;
                    //guardo si es la correcta
                    printf("%d- %s", i, (line + 4 * sizeof(char)));
                }
                else
                {
                    printf("%d- %s", i, (line + 3 * sizeof(char)));
                }
            }
        }
        //pregunto cual crees que es la correcta
        //esto despues va en cliente//
        int resp;
        printf("Respuesta: ");
        while (scanf("%d", &resp) != 1)
        {
            printf("\nPor favor ingresa un numero: ");
            while (getchar() != '\n')
                ;
        }

        if (resp == correcta)
        {
            printf("bien ahi pisculichi\n");
            puntaje++;
        }
        else
        {
            printf("NO la respuesta correcta era: %d\n", correcta);
        }
        //comparo con tu respuesta

        sleep(1);
    }

    fclose(fp);
    //cierro el file
    if (line)
        free(line);

    printf("clavaste %d puntos\n\n", puntaje );
}