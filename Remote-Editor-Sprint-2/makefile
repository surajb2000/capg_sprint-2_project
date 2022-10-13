server:
	gcc server.c serverFunctions.c user.c -o bin/server.out -g
	
run s:
	./bin/server.out

test s:
	gdb ./bin/server.out
	
client:
	gcc client.c clientFunctions.c -o bin/client.out -g
	./bin/client.out

clean : 
	rm -rf ./bin/*