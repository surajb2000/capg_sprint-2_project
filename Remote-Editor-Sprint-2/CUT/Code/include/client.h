#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

#define MAX_SIZE 1024

typedef struct Client {
    int socketfd;
    struct sockaddr_in server_addr;
    int isConnected;
    char buffer[MAX_SIZE];
} c;

int CreateSocket(c *);
int ConnectToServer(c *);
int SendDataToServer(char *data, c *c);
char *ReceiveDataFromServer(c *c);
int DisconnectClient(c *c);
int AuthenticateUser(char *username, char *password, c *c);
int EditLine(c *c);
int ReceiveFile(c *c);

#endif