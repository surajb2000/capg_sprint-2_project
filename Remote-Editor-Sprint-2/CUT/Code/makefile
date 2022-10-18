CC = gcc
serverobjs = server.o serverFunctions.o user.o
clientobjs = client.o clientFunctions.o
bin = ./bin
obj = ./obj
src = ./src
flags = -g

all: ${serverobjs} ${clientobjs}
	${CC} ${obj}/server.o ${obj}/serverFunctions.o ${obj}/user.o -o ${bin}/server.out ${flags}
	${CC} ${obj}/client.o ${obj}/clientFunctions.o -o ${bin}/client.out ${flags}


server.o:
	${CC} -c ${src}/server.c -o ${obj}/server.o ${flags}

serverFunctions.o:
	${CC} -c ${src}/serverFunctions.c -o ${obj}/serverFunctions.o ${flags}
	
user.o:
	${CC} -c ${src}/user.c -o ${obj}/user.o ${flags}

client.o:
	${CC} -c ${src}/client.c -o ${obj}/client.o ${flags}

clientFunctions.o:
	${CC} -c ${src}/clientFunctions.c -o ${obj}/clientFunctions.o ${flags}

runC:
	${bin}/client.out

runS:
	${bin}/server.out

clean : 
	rm -rf ${bin}/* ${obj}/*