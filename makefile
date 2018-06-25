all: cliente servidor

cliente:
	g++ client.c -lpthread -o cliente

servidor:
	g++ server.c -lpthread -o server

clean:
	rm cliente servidor 2> /dev/null
	
