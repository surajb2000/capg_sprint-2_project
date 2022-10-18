#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //For Sockets
#include <unistd.h>     //for fork

#include "../include/server.h"
#include "../include/user.h"

/* TCP server driver program */

void signalHandler(int signum) {
    if (signum == SIGINT) {
        printf("\n......Server shutdown!!!.....\n");
        exit(0);
    }
}

int main() {
    // create server object
    signal(SIGINT, signalHandler);
    ser server;
    createServer(&server);
    LoadUsersData(&server);
    while (1) {
        // accept connection from client and store the socket descriptor
        int clientfd = AcceptConnections(&server);
        // send confirmation message to client
        SendDataToClient(clientfd, "Connected to server");
        // create child process
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        else if (pid == 0) {
            //  receive data from client
            char data[100] = "";
            strcpy(data, ReceiveDataFromClient(clientfd, &server));
            char command[100] = "", name[100] = "", password[100] = "", dir[100] = "", filename[100] = "";
            char *token = strtok(data, " ");
            token != NULL ? strcpy(command, token) : strcpy(command, "bye");
            token = strtok(NULL, " ");
            token != NULL ? strcpy(name, token) : strcpy(command, "bye");
            token = strtok(NULL, " ");
            token != NULL ? strcpy(password, token) : strcpy(command, "bye");

            user current_user;
            User(name, password, &current_user);

            /*
             * AUTHENTICATE current_user
             */
            if (AuthenticateUser(clientfd, &current_user, &server)) {
                while (1) {
                    /*
                     * clear the buffer
                     */

                    strcpy(data, "");
                    strcpy(dir, current_user.dir);

                    /*
                     * Receive request from the client
                     */

                    strcpy(data, ReceiveDataFromClient(clientfd, &server));
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

                    if (strcmp(command, "ls") == 0) {
                        ListDirContents(clientfd, dir);
                    } else if (strcmp(command, "pwd") == 0) {
                        SendDataToClient(clientfd, dir);
                    } else if (strcmp(command, "cd") == 0) {
                        char new_dir[100] = "";
                        token = strtok(NULL, " ");
                        if (token != NULL)
                            strcpy(new_dir, token);
                        ChangeDir(new_dir, &current_user, clientfd, &server);
                    } else if (strcmp(command, "edit") == 0) {
                        int line_number;
                        token = strtok(NULL, " ");
                        line_number = atoi(token);
                        if (strcmp(filename, "") == 0)
                            SendDataToClient(clientfd, "0");
                        else
                            EditLine(clientfd, filename, line_number, &server);
                    } else if (strcmp(command, "print") == 0) {
                        if (strcmp(filename, "") == 0)
                            SendDataToClient(clientfd, "0");
                        else {
                            int start_line = 1, end_line = -1;
                            token = strtok(NULL, " ");
                            if (token != NULL) {
                                start_line = atoi(token);
                            }
                            token = strtok(NULL, " ");
                            if (token != NULL) {
                                end_line = atoi(token);
                            }
                            ViewFile(clientfd, filename, start_line, end_line, &server);
                        }
                    } else if (strcmp(command, "select") == 0) {
                        token = strtok(NULL, " ");
                        strcpy(filename, token);
                        if (strcmp(filename, "") != 0)
                            SelectFile(filename, dir, clientfd);
                    } else if (strcmp(command, "bye") == 0) {
                        printf("Client with id %d Disconnected\n", server.s);
                        close(clientfd);
                        exit(0);
                        // break;
                    } else {
                        SendDataToClient(clientfd, "Invalid command");
                    }
                }
            } else {
                close(clientfd);
                fflush(stdout);
                printf("Authentication Failed for client %d\n", server.s);
                exit(0);
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
