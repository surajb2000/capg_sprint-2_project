#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "./include/server.h"
#include "./include/user.h"

#define DATA_DIR "./data/"
#define USERS "users.txt"

#define PORT 8021
/*
 * This is the constructor for the server class.
 * It is responsible for initializing the server.
 */
int createServer(ser *ser) {
    ser->socketfd = 0;
    ser->client_addr_size = sizeof(ser->client_addr);
    ser->n = 0;
    ser->s = 0;

    /* create socket */
    ser->socketfd = socket(AF_INET, SOCK_STREAM, 0);
    // ser->socketfd = socket(AF_INET, SO_REUSEADDR, 0);
    int enable = 1;
    setsockopt(ser->socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    if (ser->socketfd == -1) {
        /* display error to stderr */
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /* initialize server address */
    ser->server_addr.sin_family = AF_INET;
    ser->server_addr.sin_port = htons(PORT);
    ser->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind socket to server address */
    if (bind(ser->socketfd, (struct sockaddr *)&ser->server_addr, sizeof(ser->server_addr)) == -1) {
        /* display error to stderr */
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* listen for connections */
    if (listen(ser->socketfd, 5) == -1) {
        /* display error to stderr */
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int LoadUsersData(ser *ser) {

    /* read data from ../data/users.txt file and store it in users vector */
    FILE *file;
    char line[100];
    char users_file[10] = USERS;
    char name[100], password[100];

    char f[100] = DATA_DIR;
    strcat(f, users_file);

    file = fopen(f, "r");
    if (file == NULL)
        return -1;
    while (fgets(line, 100, file)) {
        char *token = strtok(line, " ");
        strcpy(name, token);
        token = strtok(NULL, " ");
        strcpy(password, token);
        password[strlen(password) - 1] = '\0';
        if (password[strlen(password) - 1] == '\r') {
            password[strlen(password) - 1] = '\0';
        }
        strcpy(ser->users[ser->n].name, name);
        strcpy(ser->users[ser->n].password, password);
        ser->n++;
    }
    fclose(file);
    return 0;
}

/*
 * This function is responsibe for accepting the client connection.
 */
int AcceptConnections(ser *ser) {
    /* accept connection from client */
    int client_socketfd = accept(ser->socketfd, (struct sockaddr *)&ser->client_addr, &ser->client_addr_size);
    if (client_socketfd == -1) {
        /* display error to stderr */
        perror("accept");
        return -1;
    }
    ser->s++;
    printf("Client with id %d Connected\n", ser->s);
    return client_socketfd;
}

/*
 * This receive function is responsible for receiving the data from the client.
 */
char *ReceiveDataFromClient(int client_socketfd, ser *ser) {
    /* receive data from client */
    memset(ser->buffer, 0, sizeof(ser->buffer));
    if (recv(client_socketfd, ser->buffer, MAX_SIZE, 0) == -1) {
        /* display error to stderr */
        perror("recv");
        return "";
    }
    return ser->buffer;
}

/*
 * This function is responsible for sending data to the client.
 */
int SendDataToClient(int client_socketfd, const char *data) {
    /* send data to client */
    if (send(client_socketfd, data, strlen(data), 0) == -1) {
        /* display error to stderr */
        perror("send");
        return -1;
    }
    return 0;
}

/*
 * This function is responsible for handling the authentication of the client.
 * It is responsible for checking if the client is authenticated or not.
 * It is responsible for checking if the client is blacklisted or not.
 */

int AuthenticateUser(int client_socketfd, const user *current_user, ser *ser) {
    /* check whether user is present in users vector */
    int n = ser->n;
    for (int i = 0; i < n; i++) {
        if (strcmp(ser->users[i].name, current_user->name) == 0 && strcmp(ser->users[i].password, current_user->password) == 0) {
            /* send success message to client */
            send(client_socketfd, "AUTHENTICATED", strlen("AUTHENTICATED"), 0);
            return 1;
        }
    }
    /* send failure message to client */
    send(client_socketfd, "NOT_AUTHENTICATED", strlen("NOT_AUTHENTICATED"), 0);
    return 0;
}

/*
 * This function is responsible for handling the ls request.
 */

int ListDirContents(int client_socketfd, const char *directory) {
    /* list directory contents */
    DIR *dir;
    char buffer[1000] = {"\0"}, filename[1000] = {"\0"};
    dir = opendir(directory);
    if (dir != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
                // filename = 0;
                /* send the directory contents to client */
                // dont add .. and . to the list
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                    strcpy(filename, "d\t");
                    strcat(filename, ent->d_name);
                    strcat(filename, "/\n");
                    strcat(buffer, filename);
                    // filename = "d\t" + (char)ent->d_name + "/\n";
                    // buffer += filename;
                }
            } else if (ent->d_type == DT_REG) {
                strcpy(filename, "-\t");
                strcat(filename, ent->d_name);
                strcat(filename, "\n");
                strcat(buffer, filename);
                // filename = "-\t" + std::string(ent->d_name) + "\n";
                // buffer += filename;
            }
        }
        /* remove the last newline character */
        buffer[strlen(buffer) - 1] = '\0';
        if (strcmp(buffer, "") == 0) {
            strcpy(buffer, "Empty Directory\n");
        }
        SendDataToClient(client_socketfd, buffer);
        closedir(dir);
        return 0;
    }
    send(client_socketfd, "NO_FILES_FOUND", sizeof("NO_FILES_FOUND"), 0);
    return -1;
}

/*
 * This function is responsible for handling the cd request.
 */

int ChangeDir(const char *new_directory, user *current_user, int client_socketfd, ser *ser) {
    DIR *dir;
    if (strcmp(new_directory, "") == 0) {
        /* change directory to home directory */
        ChangeUserDir(current_user, "");
        /* send switched to home directory message to client */
        send(client_socketfd, "SWITCHED_TO_HOME_DIRECTORY", sizeof("SWITCHED_TO_HOME_DIRECTORY"), 0);
        return 0;
    } else if ((dir = opendir(current_user->dir)) != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, new_directory) == 0 && strcmp(new_directory, "..") != 0 && strcmp(new_directory, ".") != 0) {
                /* change directory to new directory */
                char temp[100] = "";
                strcpy(temp, current_user->dir);
                strcat(temp, "/");
                strcat(temp, new_directory);
                ChangeUserDir(current_user, temp);
                /* send success message to client */
                send(client_socketfd, "DIRECTORY_CHANGED", sizeof("DIRECTORY_CHANGED"), 0);
                closedir(dir);
                return 0;
            } else if (strcmp(new_directory, "..") == 0 || strcmp(new_directory, "../") == 0) {
                /* change directory and restrict user to go beyond the ../data/home/username directory */
                char temp[100] = "";
                strcpy(temp, "./data/home/");
                strcat(temp, current_user->name);
                if (strcmp(current_user->dir, temp) == 0) {
                    /* send restricted message to client */
                    send(client_socketfd, "DIRECTORY_RESTRICTED", sizeof("DIRECTORY_RESTRICTED"), 0);
                    closedir(dir);
                    return 0;
                }
                /* go to previous directory and remove the last directory from the current directory */
                int l = strlen(current_user->dir);
                while (l--) {
                    if (current_user->dir[l] == '/') {
                        current_user->dir[l] = '\0';
                        break;
                    }
                }
                //./data/home/suraj/some
                //./data/home/suraj
                // ChangeUserDir(current_user, temp);
                /* send success message to client */
                send(client_socketfd, "DIRECTORY_CHANGED", sizeof("DIRECTORY_CHANGED"), 0);
                closedir(dir);
                return 0;
            }
        }
        closedir(dir);
    }
    /* send failure message to client */
    send(client_socketfd, "DIRECTORY_NOT_FOUND", sizeof("DIRECTORY_NOT_FOUND"), 0);
    return -1;
}

/*
 * This function is responsible for handling the select request.
 */
int SelectFile(char *filename, const char *dirname, int client_socketfd) {
    /* check whether file exists in current directory */
    DIR *dir;
    if ((dir = opendir(dirname)) != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG && strcmp(ent->d_name, filename) == 0) {
                /* set the filename to fully qualified path */
                char new[100] = "";
                strcpy(new, dirname);
                strcat(new, "/");
                strcat(new, filename);
                strcpy(filename, new);
                /* send success message to client */
                send(client_socketfd, "FILE_SELECTED", sizeof("FILE_SELECTED"), 0);
                closedir(dir);
                return 0;
            }
        }
        closedir(dir);
    }
    /* clear filename */
    strcpy(filename, "");
    /* send failure message to client */
    send(client_socketfd, "FILE_NOT_FOUND", sizeof("FILE_NOT_FOUND"), 0);
    return -1;
}

/*
 * This function is responsible for handling the edit request.
 */
int EditLine(int client_socketfd, const char *filename, int line_number, ser *ser) {
    /* open file in read mode at line_number line */
    // std::ifstream file(filename);
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        SendDataToClient(client_socketfd, "FILE_NOT_FOUND");
        return -1;
    }
    /* store the file in a vector */
    char lines[100][1000];
    for (int i = 0; i < 100; i++) {
        memset(lines[i], 0, sizeof(lines[i]));
    }
    // fseek(f, 0, SEEK_SET);
    char line[1000] = "";
    int i = 0;
    while (fgets(line, 1000, f)) {
        strcpy(lines[i], line);
        i++;
        // lines.push_back(line);
    }
    /* close file */
    // file.close();
    /* check whether line_number is valid */
    // fseek(f, 0, SEEK_SET);
    fclose(f);
    if (line_number > i) {
        /* send failure message to client */
        SendDataToClient(client_socketfd, "INVALID_LINE_NUMBER");
        return -1;
    }

    /* send the selected line with line_number to client */
    // line.clear();
    strcpy(line, "");
    int space_count = 0;
    char ch;
    for (int k = 0; k < strlen(lines[line_number - 1]); k++) {
        ch = lines[line_number - 1][k];
        if (ch == ' ') {
            space_count++;
        } else {
            break;
        }
    }
    char trimmed_line[100];
    // = lines[line_number - 1][space_count];
    strcpy(trimmed_line, lines[line_number - 1] + space_count);
    sprintf(line, "%d", line_number);
    strcat(line, ":");
    strcat(line, trimmed_line);
    SendDataToClient(client_socketfd, line);

    /* receive the edited line from client */
    memset(ser->buffer, 0, sizeof(ser->buffer));
    recv(client_socketfd, ser->buffer, sizeof(ser->buffer), 0);

    printf("Changes received from client: %s\n", ser->buffer);

    // // again load the file in a vector
    // std::ifstream file1(filename);
    // if (!file1.is_open()) {
    //     /* display error to stderr */
    //     SendDataToClient(client_socketfd, "FILE_NOT_FOUND");
    //     return -1;
    // }
    // std::string line1;
    // lines.clear();
    // while (std::getline(file1, line1)) {
    //     lines.push_back(line1);
    // }
    // /* close file */
    // file1.close();

    /* replace the line in the vector */
    if (strcmp(ser->buffer, "0") != 0) {
        strcpy(trimmed_line, "");
        while (space_count--) {
            strcat(trimmed_line, " ");
        }
        strcat(trimmed_line, ser->buffer);
        strcpy(lines[line_number - 1], trimmed_line);
    }

    /* open file in write mode */
    // std::ofstream file_write(filename);

    // /* write the vector to file */
    // for (auto single_line : lines) {
    //     file_write << single_line << std::endl;
    // }
    // /* close file */
    // file_write.close();
    f = fopen(filename, "w");
    for (int k = 0; k < i; k++) {
        fputs(lines[k], f);
    }
    fclose(f);
    return 0;
}

/*
 * This function is responsible for handling the print request.
 */
int ViewFile(int client_socketfd, const char *filename, int start_line, int end_line, ser *ser) {
    /* open file in read mode */
    FILE *f = fopen(filename, "r");
    // std::ifstream file(filename);
    if (f == NULL) {
        /* end failure message to client */
        SendDataToClient(client_socketfd, "FILE_NOT_FOUND");
        return -1;
    }

    // get the number of lines in the file
    char line[100] = "";
    int line_number = 0;
    // std::ifstream file_read(filename);
    //  while (std::getline(file_read, line)) {
    //      line_number++;
    //  }
    //  file_read.close();
    while (fgets(line, 100, f)) {
        line_number++;
    }
    fseek(f, 0, SEEK_SET);

    /* check whether start_line and end_line is valid */
    if (end_line > line_number || start_line > line_number) {
        // send the failure msg with start and end line of the file to client
        char msg[100] = "INVALID_LINE_NUMBER : Choose between 1 and ";
        char temp[100] = "";
        sprintf(temp, "%d", line_number);
        strcat(msg, temp);
        SendDataToClient(client_socketfd, msg);

    } else {

        /* read file contents and send it to the client */
        strcpy(line, "");
        int i = 1;
        while (fgets(line, 100, f)) {
            if (i >= start_line) {
                /* send line with line number to client */
                char line_with_number[1000];
                char temp[100];
                // itoa(i,temp,DECIMAL);
                sprintf(temp, "%d", i);
                strcpy(line_with_number, temp);
                strcat(line_with_number, " ");
                // sprintf(temp, "%d", line);
                strcat(line_with_number, line);
                // strcat(line_with_number, "\n");
                SendDataToClient(client_socketfd, line_with_number);
                if (end_line != -1 && i == end_line) {
                    fclose(f);
                    break;
                }
            }
            i++;
        }
        if (end_line == -1)
            fclose(f);
    }
    memset(ser->buffer, 0, sizeof(ser->buffer));
    send(client_socketfd, ser->buffer, MAX_SIZE, 0);
    return 0;
}