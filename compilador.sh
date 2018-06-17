#!/bin/bash
# COMPILADOR DEL SERVIDOR y CLIENTE, DEBE ESTAR EN LA MISMA CARPETA DE LOS ARCHIVOS .C
g++ server.c -lpthread -o server
g++ client.c -lpthread -o cliente
#Ejemplo de ejecuccion ./scriptCompilador
