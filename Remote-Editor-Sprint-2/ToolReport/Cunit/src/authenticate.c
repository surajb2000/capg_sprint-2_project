#include "../include/authenticate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int authentication(char *tempfile,char user[],char psd[]) {
        FILE *fptr = fopen((char *)tempfile, "r");
        if (fptr == NULL) {
            printf("File Error");
            exit(0);
        }
        char str[SIZE], *token, username[SIZE], password[SIZE];
        int flag=1;
        while (fgets(str, SIZE, fptr)) {
            token=strtok(str," ");
               // token[strlen(token) - 1] = '\0';
                strcpy(username,token);
                token=strtok(NULL," ");
                strcpy(password,token);
                password[strlen(password) - 1] = '\0';
                //token=strtok(NULL," ");
            printf("Username-%s-%s-\n",username,user);
            printf("password-%s-%s-\n",password,psd);
		if(strcmp(username,user)==0 && strcmp(password,psd)==0) {
		    //printf("File Username %s\n",username);
                    flag=2;
		            break;
            }
        }
        (void)fclose(fptr);
        return flag;
}
