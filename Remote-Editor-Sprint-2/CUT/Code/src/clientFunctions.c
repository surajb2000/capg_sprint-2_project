/********************************************
 * *FILENAME	      : clientFunctions.c
 *
 * *DESCRITION        : This file defines the functions that are used to process our commands from 
 *                      client end.
 *
 *
 * Revision History   :	       
 *
 * 	Date			Name			Reason
 *
 * 27th Aug 2022		----			-----
 *
 *
*********************************************/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*user defined headers*/
#include <client.h>

#define PORT 8022
#define IP "127.0.0.1"

// Initializing the client variables
/********************************************
 * *FUNCTION NAME : CreateSocket
 *
 * *DESCRIPTION   : This function is used to create a socket using socket system call, and 
 *                  then we initialize server address 
 *
 *
 * *RETURNS       : socketfd of client
 *
 *
 *
*********************************************/
int CreateSocket(c *c) {
    /* create socket */
    c->isConnected = 0;
    c->socketfd = 0;
    c->socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (c->socketfd == -1) {
        /* display error to stderr */
        perror("socket");
        return -1;
    }
    /* initialize server address */
    c->server_addr.sin_family = AF_INET;
    c->server_addr.sin_port = htons(PORT);
    c->server_addr.sin_addr.s_addr = inet_addr(IP);
    return c->socketfd;
}
/********************************************
 * *FUNCTION NAME : ConnectToServer
 *
 * *DESCRIPTION   : This function is responsible to connect client to server using connect system call.
 *                  After connection we are changing the value of isConnected to 1.
 *
 *
 *
 * *RETURNS       : isConnected value of client
 *
 *
 *
*********************************************/
int ConnectToServer(c *c) {
    /* connect to server */
    if (connect(c->socketfd, (struct sockaddr *)&c->server_addr, sizeof(c->server_addr)) == -1) {
        /* display error to stderr */
        perror("connect");
        return -1;
    }
    c->isConnected = 1;
    return 0;
}
/********************************************
 * *FUNCTION NAME : SendDataToServer
 *
 * *DESCRIPTION   : This function is responsible to send the data to server.
 *
 *
 *
 * *RETURNS       : returns 0 if successfully sent
 *
 *
 *
*********************************************/
int SendDataToServer(char *data, c *c) {
    /* send data to server */

    if (strcmp(data, "") != 0 && send(c->socketfd, data, strlen(data), 0) == -1) {
        /* display error to stderr */
        perror("send");
        return -1;
    }
    return 0;
}
/********************************************
 * *FUNCTION NAME : ReceiveDataFromServer
 *
 * *DESCRIPTION   : This function is responsible to receive data from server via the buffer char array.
 *
 *
 *
 * *RETURNS       : returns buffer char array of client 
 *
 *
 *
*********************************************/
char *ReceiveDataFromServer(c *c) {
    memset(c->buffer, 0, sizeof(c->buffer));
    if (recv(c->socketfd, c->buffer, sizeof(c->buffer), 0) == -1) {
        /* display error to stderr */
        perror("recv");
        exit(EXIT_FAILURE);
    }
    /* display data to stdout */
    printf("%s\n", c->buffer);
    return c->buffer;
}
/********************************************
 * *FUNCTION NAME : AuthenticateUser
 *
 * *DESCRIPTION   : This function is responsible to authenticate user.
 *                  Check whether client is connected.
                    Inside this function we are using a data variable and storing AUTHENTICATE 
                    and then username and password.
                    After that we are sending this data to server to process authentication at 
                    server end.
                    If server sends message as AUTHENTICATED in buffer then the credentials matched.
 *
 *
 *
 * *RETURNS       : returns 1 if authenticated 
 *
 *
 *
*********************************************/
int AuthenticateUser(char *username, char *password, c *c) {
    /* check whether the client is connected to server */
    if (!c->isConnected) {
        /* display error to stderr */
        printf("Not connected to the server");
        // std::cerr << "Not connected to the server" << std::endl;
        exit(EXIT_FAILURE);
    }
    /* authenticate user */
    char data[MAX_SIZE] = "AUTHENTICATE ";
    strcat(data, username);
    strcat(data, " ");
    strcat(data, password);

    /* send data to server */
    if (SendDataToServer(data, c) == -1) {
        printf("Error in sending data to server\n");
    }

    /* receive data from server */
    memset(c->buffer, 0, sizeof(c->buffer));
    if (recv(c->socketfd, c->buffer, sizeof(c->buffer), 0) == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    /* check whether the user is authenticated */
    if (strcmp(c->buffer, "AUTHENTICATED") == 0)
        return 1;
    c->isConnected = 0;
    return 0;
}
/********************************************
 * *FUNCTION NAME : EditLine
 *
 * *DESCRIPTION   : In this function we first compare the value of buffer of client
                    If the value is 0 then file is not selected
                    If the value is Invalid line number then same message is shown.
                    After that we ask the user to enter the changes in file .
                    After we have received the input Send the Data to Server.

 *
 *
 *
 * *RETURNS       : returns 0
 *
 *
 *
*********************************************/
int EditLine(c *c) {
    memset(c->buffer, 0, sizeof(c->buffer));
    int bytes_read = read(c->socketfd, c->buffer, sizeof(c->buffer));
    if (strcmp("0", c->buffer) == 0) {
        printf("FILE_NOT_SELECTED: use select <FILENAME> command\n");
        return -1;
    }
    if (strcmp("INVALID_LINE_NUMBER", c->buffer) == 0) {
        printf("INVALID_LINE_NUMBER\n");
        return -1;
    }
    printf("%s", c->buffer);

    /* ask user to edit the line received from server and send it to server */
    char edited_line[MAX_SIZE];
    printf("Enter changes to the line: ");
    fgets(edited_line, MAX_SIZE, stdin);
    if (strlen(edited_line) == 0)
        SendDataToServer("0", c);
    else
        SendDataToServer(edited_line, c);
    return 0;
};
/********************************************
 * *FUNCTION NAME : DisconnectClient
 *
 * *DESCRIPTION   : This function is responsible to send bye to server and close the socket
 *
 *
 *
 * *RETURNS       : returns 0
 *
 *
 *
*********************************************/
int DisconnectClient(c *c) {
    /* send "bye" to server and close the socket */
    SendDataToServer("bye", c);
    close(c->socketfd);
    c->isConnected = 0;
    return 0;
}
/********************************************
 * *FUNCTION NAME : ReceiveFile
 *
 * *DESCRIPTION   : This function is responsible to check whether file is selected or not.
                    We receive the information in buffer.
                    If the buffer value of c is 0 then that means FILE is not selected
                    Otherwise File is selected.

 *
 *
 *
 * *RETURNS       : returns 0
 *
 *
 *
*********************************************/
int ReceiveFile(c *c) {
    int isNotEnd = 1;
    while (isNotEnd) {
        int len = 0;
        /* receive data from server */
        memset(c->buffer, 0, sizeof(c->buffer));
        len = recv(c->socketfd, c->buffer, sizeof(c->buffer), 0);
        if (strcmp("0", c->buffer) == 0) {
            printf("FILE_NOT_SELECTED : use select <FILENAME> command\n");
            return -1;
        }
        isNotEnd = c->buffer[0];
        printf("%s\n", c->buffer);
    }
    printf("\n");
    return 0;
}