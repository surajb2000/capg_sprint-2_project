#include "./include/client.h"
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
// include

#define DATA_DIR "../data/"
#define USERS "users.txt"
#define PORT 8021

/*

struct sockaddr_in server; // This is our main socket variable.
int fd;                    // This is the socket file descriptor that will be used to identify the socket
int conn;                  // This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = "";    // This array will store the messages that are sent by the server

int main() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(8012);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);       // This binds the client to localhost
    connect(fd, (struct sockaddr *)&server, sizeof(server)); // This connects the client to the server.
    while (1) {
        printf("Enter a message: ");
        fgets(message, 100, stdin);
        send(fd, message, strlen(message), 0);
        // An extra breaking condition can be added here (to terminate the while loop)
    }
}
*/

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
        sockid = GetSocketfd(&client);

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
                printf("AUTHENTICATED\n");
            else {
                printf("\nAnonumous\n");
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
                /* remove trailing space */
                command[strlen(command) - 1] = '\0';

                /* separate command and arguments */
                char *subcommand = strtok(command, " ");

                if (strcmp(subcommand, "ls") == 0 || strcmp(subcommand, "pwd") == 0 && arguments == 0) {
                    SendDataToServer(command, &client);
                    // } else if (strcmp(subcommand, "help") && arguments == 0) {
                    //     printf("List of commands available: ls pwd cd select print edit clear bye help\n");
                    //     continue;
                    // } else if (subcommand == "help" && arguments == 1) {
                    //     char *help_command = strtok(NULL, " ");
                    //     if (help_command == "ls") {
                    //         printf("ls - list files in current directory\nNo arguments are needed\n");
                    //     } else if (help_command == "pwd") {
                    //         printf("pwd - print current directory\nNo arguments are needed\n");
                    //     } else if (help_command == "cd") {
                    //         printf("cd <directory> - change current directory\nIt takes directory name as argument\n");
                    //     } else if (help_command == "select") {
                    //         printf("select <filename> - select file to edit\nIt takes filename name as argument\n");
                    //     } else if (help_command == "print") {
                    //         printf("print <start> <end> - print the file\nIt takes 2 numbers (starting and ending number) as optional arguments\n");
                    //     } else if (help_command == "edit") {
                    //         printf("edit <linenum> - edit the file\nIt takes line number as arguments and edit the line on the selected file\n");
                    //     } else if (help_command == "bye") {
                    //         printf("bye - logout the current user\nNo arguments are needed\n");
                    //     } else if (help_command == "help") {
                    //         printf("help - print list of commands\nsubcommand are passed as arguments\n");
                    //     } else if (help_command == "clear") {
                    //         printf("clear - clear the screen\nNo arguments are needed\n");
                    //     } else {
                    //         printf("Invalid command\n");
                    //     }
                    //     continue;
                    // } else if (subcommand == "cd" && arguments <= 1) {
                    //     SendDataToServer(command,&client);
                    // } else if (subcommand == "print" && arguments <= 2) {
                    //     if (arguments == 0) {
                    //         SendDataToServer(command,&client);
                    //     } else if (arguments == 1) {
                    //         /* check 2nd argument is a number */
                    //         std::stringstream str_stream(command.substr(command.find(" ") + 1));
                    //         int number;
                    //         str_stream >> number;
                    //         if (str_stream.fail()) {
                    //             std::cerr << "Value must be a number" << std::endl;
                    //             continue;
                    //         }
                    //         if (number <= 0) {
                    //             std::cerr << "Value should be greater than 0" << std::endl;
                    //             continue;
                    //         }
                    //         /* send command to server */
                    //         client.SendDataToServer(command);
                    //     } else {
                    //         /* check both 2nd and 3rd arguments are numbers */
                    //         int number1, number2;
                    //         std::stringstream ss1(command.substr(command.find(" ") + 1));
                    //         ss1 >> number1;
                    //         if (ss1.fail()) {
                    //             std::cerr << "Value 1 must be a number" << std::endl;
                    //             continue;
                    //         }
                    //         if (number1 <= 0) {
                    //             std::cerr << "Value 1 should be greater than 0" << std::endl;
                    //             continue;
                    //         }
                    //         ss1 >> number2;
                    //         if (ss1.fail()) {
                    //             std::cerr << "Value 2 must be a number" << std::endl;
                    //             continue;
                    //         }
                    //         if (number2 <= 0) {
                    //             std::cerr << "Value 2 should be greater than 0" << std::endl;
                    //             continue;
                    //         }
                    //         /* send command to server */
                    //         client.SendDataToServer(command);
                    //     }
                    // } else if (subcommand == "edit" && arguments == 1) {
                    //     /* check whether the argument is a number */
                    //     std::stringstream ss3(command.substr(command.find(" ") + 1));
                    //     int number;
                    //     ss3 >> number;
                    //     if (ss3.fail()) {
                    //         std::cerr << "Value must be a number" << std::endl;
                    //         continue;
                    //     }
                    //     if (number <= 0) {
                    //         std::cerr << "Value should be greater than 0" << std::endl;
                    //         continue;
                    //     }
                    //     client.SendDataToServer(command);
                    // } else if (subcommand == "select" && arguments == 1) {
                    // client.SendDataToServer(command);
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
            printf("AUTHENTICATION_FAILED try again...\n");
            DisconnectClient(&client);
        }

        return 0;
    }
}
