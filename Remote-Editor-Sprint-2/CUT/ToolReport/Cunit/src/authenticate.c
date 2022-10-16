#include "../include/authenticate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int authentication(char *tempfile, char user[], char psd[]) {
    FILE *fptr = fopen((char *)tempfile, "r");
    if (fptr == NULL) {
        printf("File Error");
        exit(0);
    }
    char str[SIZE], *token, username[SIZE], password[SIZE];
    int flag = 1;
    while (fgets(str, SIZE, fptr)) {
        token = strtok(str, " ");
        strcpy(username, token);
        token = strtok(NULL, " ");
        strcpy(password, token);
        password[strlen(password) - 1] = '\0';
        if (strcmp(username, user) == 0 && strcmp(password, psd) == 0) {
            flag = 2;
            break;
        }
    }
    (void)fclose(fptr);
    return flag;
}
