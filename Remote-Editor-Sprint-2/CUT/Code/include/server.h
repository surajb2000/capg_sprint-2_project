#ifndef SERVER_H
#define SERVER_H

#include "user.h"
#include <netinet/in.h>
#define MAX_SIZE 1024

typedef struct Server {
    int socketfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    user users[100];
    int n;
    int s;
    char buffer[MAX_SIZE];
} ser;

int createServer(ser *);
int LoadUsersData(ser *);
int AcceptConnections(ser *);
char *ReceiveDataFromClient(int clientfd, ser *);
int SendDataToClient(int clientfd, const char *data);
int AuthenticateUser(int clientfd, const user *user, ser *);
int ListDirContents(int clientfd, const char *dir);
int ChangeDir(const char *new_directory, user *current_user, int client_socketfd, ser *);
int EditLine(int clientfd, const char *file, int linenum, ser *);
int ViewFile(int clientfd, const char *file, int start, int end, ser *);
int SelectFile(char *filename, const char *dir, int clientfd);

#endif