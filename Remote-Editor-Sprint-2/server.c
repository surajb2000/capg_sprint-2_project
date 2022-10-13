#include "./include/server.h"
#include "./include/user.h"
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //For Sockets
#include <unistd.h>     //for fork

/*
struct sockaddr_in serv; // This is our main socket variable.
int fd;                  // This is the socket file descriptor that will be used to identify the socket
int conn;                // This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = "";  // This array will store the messages that are sent by the server


int main(int argc, char const *argv[]) {
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8012); // Define the port at which the server will listen for connections.
    serv.sin_addr.s_addr = INADDR_ANY;
    fd = socket(AF_INET, SOCK_STREAM, 0); // This will create a new socket and also return the identifier of the socket into fd.
    // To handle errors, you can add an if condition that checks whether fd is greater than 0. If it isn't, prompt an error
    bind(fd, (struct sockaddr *)&serv, sizeof(serv)); // assigns the address specified by serv to the socket
    listen(fd, 5);                                    // Listen for client connections. Maximum 5 connections will be permitted.
    // Now we start handling the connections.
    while (conn = accept(fd, (struct sockaddr *)NULL, NULL)) {
        int pid;
        if ((pid = fork()) == 0) {
            while (recv(conn, message, 100, 0) > 0) {
                printf("Message Received: %s\n", message);
                // An extra breaking condition can be added here (to terminate the child process)
                strcpy(message,"");
            }
            exit(0);
        }
    }
}
*/

/* TCP server driver program */

int main() {
    // create server object
    ser server;
    createServer(&server);
    // LoadUsersData(&server);
    while (1) {
        // accept connection from client and store the socket descriptor
        int clientfd = AcceptConnections(&server);
        printf("Client Connected");
        // send confirmation message to client
        SendDataToClient(clientfd, "Connected to server");
        printf("Client Connected");
        // create child process
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        else if (pid == 0) {
            // receive data from client
            char *data = ReceiveDataFromClient(clientfd, &server);
            char *command, *name, *password, *dir, *filename;
            char *token = strtok(data, " ");
            strcpy(command, token);
            strtok(NULL, " ");
            strcpy(password, token);
            strtok(NULL, " ");
            strcpy(dir, token);
            strtok(NULL, " ");
            strcpy(filename, token);

            user current_user = User(name, password);

            /*
             * AUTHENTICATE current_user
             */
            if (AuthenticateUser(clientfd, &current_user, &server)) {
                while (1) {
                    /*
                     * clear the buffer
                     */

                    strcpy(data, "");
                    dir = GetDir(&current_user);

                    /*
                     * Receive request from the client
                     */

                    data = ReceiveDataFromClient(clientfd, &server);
                    token = strtok(data, " ");
                    strcpy(command, token);

                    /*
                     * Command List
                     * Ls
                     * cd
                     * edit
                     * view
                     * select
                     * bye
                     */

                    if (command == "ls") {
                        ListDirContents(clientfd, dir);
                    } else if (command == "pwd") {
                        SendDataToClient(clientfd, dir);
                    } else if (command == "cd") {
                        char *new_dir;
                        token = strtok(NULL, " ");
                        strcpy(new_dir, token);
                        // ChangeDir(new_dir, current_user, clientfd);
                        // } else if (command == "edit") {
                        //     int line_number;
                        //     ss >> line_number;
                        //     if (filename.empty())
                        //         server.SendDataToClient(clientfd, "0");
                        //     else
                        //         server.EditLine(clientfd, filename, line_number);
                        // } else if (command == "print") {
                        //     if (filename.empty())
                        //         server.SendDataToClient(clientfd, "0");
                        //     else {
                        //         int start_line = 1, end_line = -1;
                        //         ss >> start_line >> end_line;
                        //         server.ViewFile(clientfd, filename, start_line, end_line);
                        //     }
                        // } else if (command == "select") {
                        //     ss >> filename;
                        //     if (!filename.empty())
                        //         server.SelectFile(filename, dir, clientfd);
                    } else if (command == "bye") {
                        close(clientfd);
                        break;
                    } else {
                        SendDataToClient(clientfd, "Invalid command");
                    }
                }
            }
        } else {
            /*
             * inside parent process
             * SIGCHLD is sent to the parent process when a child process exits.
             * closing the socket.
             */
            signal(SIGCHLD, SIG_IGN);
            close(clientfd);
        }
    }
    return 0;
}
