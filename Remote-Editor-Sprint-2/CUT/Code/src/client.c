#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //For Sockets
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "../include/client.h"

#define DATA_DIR "../data/"
#define USERS "users.txt"
#define PORT 8012

/*
 * This is the source code for the client program.
 */

/* This Global variable is used for signal handler function */
int sockid;

/* Function to handle ctrl+c */
void signalHandler(int signum) {
    if (signum == SIGINT) {
        // send bye to server
        send(sockid, "bye", 3, 0);
        exit(0);
    }
}

int main() {
    while (1) {
        c client;
        CreateSocket(&client);
        sockid = client.socketfd;

        /* register the signal handler for ctrl+c */
        signal(SIGINT, signalHandler);

        /* connect to server */
        ConnectToServer(&client);

        /* receive connection confirmation from server */
        ReceiveDataFromServer(&client);

        /*
        /* get username and password from user */
        char username[100] = "", password[100] = "";
        printf("\nEnter username: ");
        fgets(username, 100, stdin);
        username[strlen(username) - 1] = '\0';

        struct termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        struct termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        printf("Enter password: ");
        fgets(password, 100, stdin);
        password[strlen(password) - 1] = '\0';
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        int u = strcmp(username, "");
        int p = strcmp(password, "");
        if (u == 0 && p == 0) {
            strcpy(username, "anonymous");
            strcpy(password, "password");
        }
        /* authenticate user */
        if (AuthenticateUser(username, password, &client)) {
            if (strcmp(username, "anonymous") != 0)
                printf("\n......AUTHENTICATED......\n");
            else {
                printf("\n......Anonymous......\n");
            }
            while (1) {
                /* get user input */
                char input[100] = "";
                printf("Enter command-$: ");
                fgets(input, 100, stdin);
                input[strlen(input) - 1] = '\0';
                if (feof(stdin)) {
                    DisconnectClient(&client);
                    exit(0);
                }

                char *token = strtok(input, " ");
                int arguments = -1;
                char command[100] = "";
                while (token != NULL) {
                    strcat(command, token);
                    strcat(command, " ");
                    arguments++;
                    token = strtok(NULL, " ");
                }
                /* remove trailing newline */
                command[strlen(command) - 1] = '\0';
                char com[100] = "";
                strcpy(com, command);

                /* separate command and arguments */
                char *subcommand = strtok(command, " ");

                if (strcmp(subcommand, "ls") == 0 || strcmp(subcommand, "pwd") == 0 && arguments == 0) {
                    SendDataToServer(command, &client);
                } else if (strcmp(subcommand, "help") == 0 && arguments == 0) {
                    printf("List of commands available: ls pwd cd select print edit clear bye help\n");
                    continue;
                } else if (strcmp(subcommand, "help") == 0 && arguments == 1) {
                    char *help_command = strtok(NULL, " ");
                    if (strcmp(help_command, "ls") == 0) {
                        printf("ls - list files in current directory\nNo arguments are needed\n");
                    } else if (strcmp(help_command, "pwd") == 0) {
                        printf("pwd - print current directory\nNo arguments are needed\n");
                    } else if (strcmp(help_command, "cd") == 0) {
                        printf("cd <directory> - change current directory\nIt takes directory name as argument\n");
                    } else if (strcmp(help_command, "select") == 0) {
                        printf("select <filename> - select file to edit\nIt takes filename name as argument\n");
                    } else if (strcmp(help_command, "print") == 0) {
                        printf("print <start> <end> - print the file\nIt takes 2 numbers (starting and ending number) as optional arguments\n");
                    } else if (strcmp(help_command, "edit") == 0) {
                        printf("edit <linenum> - edit the file\nIt takes line number as arguments and edit the line on the selected file\n");
                    } else if (strcmp(help_command, "bye") == 0) {
                        printf("bye - logout the current user\nNo arguments are needed\n");
                    } else if (strcmp(help_command, "help") == 0) {
                        printf("help - print list of commands\nsubcommand are passed as arguments\n");
                    } else if (strcmp(help_command, "clear") == 0) {
                        printf("clear - clear the screen\nNo arguments are needed\n");
                    } else {
                        printf("Invalid command\n");
                    }
                    continue;
                } else if (strcmp(subcommand, "cd") == 0 && arguments <= 1) {
                    SendDataToServer(com, &client);
                } else if (strcmp(subcommand, "print") == 0 && arguments <= 2) {
                    if (arguments == 0) {
                        SendDataToServer(command, &client);
                    } else if (arguments == 1) {
                        /* check argument is a number */
                        char *subarg1 = strtok(NULL, " ");
                        int number = atoi(subarg1);
                        if (number <= 0) {
                            printf("Value must be a number and should be greater than 0\n");
                            continue;
                        }
                        /* send command to server */
                        SendDataToServer(com, &client);
                    } else {
                        /* check both arguments are numbers */
                        char *subarg = strtok(NULL, " ");
                        int number1 = atoi(subarg);
                        subarg = strtok(NULL, " ");
                        int number2 = atoi(subarg);
                        if (number1 <= 0 || number2 <= 0) {
                            printf("Value must be a number and should be greater than 0\n");
                            continue;
                        }
                        /* send command to server */
                        SendDataToServer(com, &client);
                    }
                } else if (strcmp(subcommand, "edit") == 0 && arguments == 1) {
                    /* check whether the argument is a number */
                    char *subarg = strtok(NULL, " ");
                    int number = atoi(subarg);
                    if (number <= 0) {
                        printf("Value must be a number and should be greater than 0\n");
                        continue;
                    }
                    SendDataToServer(com, &client);
                } else if (strcmp(subcommand, "select") == 0 && arguments == 1) {
                    SendDataToServer(com, &client);
                } else if (strcmp(subcommand, "bye") == 0 && arguments == 0) {
                    DisconnectClient(&client);
                    break;
                } else if (strcmp(subcommand, "clear") == 0 || strcmp(subcommand, "c") == 0) {
                    system("clear");
                    continue;
                } else {
                    if (strcmp(subcommand, "ls") == 0 && arguments > 0) {
                        printf("ls : too many arguments\n");
                    } else if (strcmp(subcommand, "cd") == 0 && arguments > 1) {
                        printf("cd : too many arguments\n");
                    } else if (strcmp(subcommand, "print") == 0 && (arguments > 2)) {
                        printf("print : too many arguments\n");
                    } else if (strcmp(subcommand, "edit") == 0 && (arguments > 1 || arguments < 1)) {
                        if (arguments > 1) {
                            printf("edit : too many arguments\n");
                        } else {
                            printf("<LINENUM> is missing in command : edit <LINENUM>\n");
                        }
                    } else if (strcmp(subcommand, "select") == 0 && (arguments > 1 || arguments < 1)) {
                        if (arguments > 1) {
                            printf("select : too many arguments\n");
                        } else {
                            printf("<FILENAME> is missing in command : select <FILENAME>");
                        }
                    } else if (strcmp(subcommand, "bye") == 0 && arguments >= 1) {
                        printf("bye : too many arguments\n");
                    } else {
                        printf("Invalid command\n");
                    }
                    continue;
                }

                /* receive data from server */
                if (strcmp(subcommand, "edit") == 0) {
                    EditLine(&client);
                } else if (strcmp(subcommand, "print") == 0) {
                    ReceiveFile(&client);
                } else {
                    ReceiveDataFromServer(&client);
                }
            }
        } else {
            printf("\n...AUTHENTICATION_FAILED try again...\n");
            DisconnectClient(&client);
            continue;
        }
        return 0;
        // char exit = '1';
        // printf("Enter 0 to exit or any other key to continue : ");
        // scanf("%c",&exit);
        // getchar();
        // if (exit == '0') {
        //     return 0;
        // } else {
        //     system("clear");
        // }
    }
}
