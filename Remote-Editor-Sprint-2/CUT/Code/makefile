CC = gcc
SERVEROBJ = server.o serverFunctions.o user.o
CLIENTOBJ = client.o clientFunctions.o
BINDIR = ./bin
OBJDIR = ./obj
SRCDIR = ./src
INCDIR = ./include
flags = -g -c -I${INCDIR}


all: ${SERVEROBJ} ${CLIENTOBJ}
	${CC} ${OBJDIR}/server.o ${OBJDIR}/serverFunctions.o ${OBJDIR}/user.o -o ${BINDIR}/server.out
	${CC} ${OBJDIR}/client.o ${OBJDIR}/clientFunctions.o -o ${BINDIR}/client.out

server.o:
	${CC} ${flags} ${SRCDIR}/server.c -o ${OBJDIR}/server.o 

serverFunctions.o:
	${CC} ${flags} ${SRCDIR}/serverFunctions.c -o ${OBJDIR}/serverFunctions.o 
	
user.o:
	${CC} ${flags} ${SRCDIR}/user.c -o ${OBJDIR}/user.o

client.o:
	${CC} ${flags} ${SRCDIR}/client.c -o ${OBJDIR}/client.o

clientFunctions.o:
	${CC} ${flags} ${SRCDIR}/clientFunctions.c -o ${OBJDIR}/clientFunctions.o

runC:
	${BINDIR}/client.out

runS:
	${BINDIR}/server.out

clean : 
	rm -rf ${BINDIR}/* ${OBJDIR}/*