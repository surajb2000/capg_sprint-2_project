CC = gcc
sfolder = src
ofolder = bin
flags = -lcunit -g
objs = main.o authenticate.o authenticate_suite.o 

main.c : $(objs)
	${CC} bin/main.o bin/authenticate.o bin/authenticate_suite.o -o bin/file.tests ${flags}
	./bin/file.tests

main.o: src/main.c
	${CC} -c src/main.c -o bin/main.o ${flags}

authenticate.o: src/authenticate.c
	${CC} -c src/authenticate.c -o bin/authenticate.o ${flags}

authenticate_suite.o: src/authenticate_suite.c
	${CC} -c src/authenticate_suite.c -o bin/authenticate_suite.o ${flags}
