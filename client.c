/*
    C ECHO client example using sockets
*/
#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];
    char separator[5] = "|", chau[100] = "gracias, vuelva pronto", *flag = NULL;

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while (1)
    {
        memset(server_reply, '\0', sizeof(server_reply));
        if (recv(sock, server_reply, 2000, 0) < 0)
        {
            puts("recv failed");
            break;
        }
        flag = strstr(server_reply, separator);
        while (flag == NULL)
        {
            puts(server_reply);

            memset(server_reply, '\0', sizeof(server_reply));
            if (recv(sock, server_reply, 2000, 0) < 0)
            {
                puts("recv failed");
                break;
            }
            flag = strstr(server_reply, separator);
        }
        puts(server_reply);
        flag = NULL;
        flag = strstr(server_reply, chau);
        if (flag)
        {
            break;
        }

        printf("\nEnter message : ");
        scanf("%s", message);

        //Send some data
        if (send(sock, message, strlen(message), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
    }

    close(sock);
    return 0;
}