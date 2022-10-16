/*#include "../include/load.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int LoadUsersData(ser *ser, char *filename) {
    FILE *file=fopen((char *)filename,"r");
    char line[100];
    //char users_file[100] = USERS;
    char name[100], password[100];

    //char f[100] = DATA_DIR;
    //strcat(f, users_file);

    //file = fopen(f, "r");
    int flag=1;
    if (file == NULL) {
        flag=0;
	    return(0);
    }
    while (fgets(line, 100, file)) {
        char *token = strtok(line, " ");
        strcpy(name, token);
        token = strtok(NULL, " ");
        strcpy(password, token);
        password[strlen(password) - 1] = '\0';

        strcpy(ser->users[ser->n].name, name);
        strcpy(ser->users[ser->n].password, password);
        ser->n++;
    }
    int n=0;
    while (fgets(line, 100, file)) {
        char *token=strtok(line," ");
        if(strcmp(token,ser->users[ser->n].name)!=0) {
            flag=0;
        }
        token = strtok(NULL, " ");
        token[strlen(token)-1]='\0';
        if(strcmp(token,ser->users[ser->n].password)!=0) {
            flag=0;
        }
        ser->n++;
    }
    fclose(file);
    return flag;
}*/