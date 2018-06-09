/*
    C socket server example, handles multiple clients using threads
*/

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>   //for threading , link with lpthread

//the thread function
void *connection_handler(void *);

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        pthread_join(sniffer_thread, NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    char *message, client_message[2000];
    //
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int puntaje = 0;
    char mensaje[200];
    //
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
            sprintf(mensaje, "\nPREGUNTA: %s", (line + 3 * sizeof(char)));
            write(sock, mensaje, strlen(message)); //manda la pregunta

            while ((read = getline(&line, &len, fp)) != -1 && *line == 'R')
            {
                //ahora mientras lleguen respuestas las mando
                i++;
                if (*(line + 1 * sizeof(char)) == 'C')
                {
                    correcta = i;
                    //guardo si es la correcta
                    sprintf(mensaje, "%d- %s", i, (line + 4 * sizeof(char)));
                    write(sock, mensaje, strlen(message));
                }
                else
                {
                    sprintf(mensaje, "%d- %s", i, (line + 3 * sizeof(char)));
                    write(sock, mensaje, strlen(message));
                }
            }
        }

        read_size = recv(sock, client_message, 2000, 0);
        if (read_size == 0)
        {
            puts("se desconecto un jugador");
            fflush(stdout);
        }
        else if (read_size == -1)
        {
            perror("error de recv");
        }

        int resp = 0;

        if (resp == correcta)
        {
            puntaje++;
        }
        //comparo con tu respuesta

        sleep(1);
    }

    fclose(fp);
    //cierro el file
    if (line)
        free(line);

    //Free the socket pointer
    close(sock);
    free(socket_desc);

    return 0;
}
