#!/bin/bash
# COMPILADOR DEL SERVIDOR y CLIENTE, DEBE ESTAR EN LA MISMA CARPETA DE LOS ARCHIVOS .C
gcc server.c -lpthread -o server
gcc client.c -lpthread -o cliente
#Ejemplo de ejecuccion ./scriptCompilador
