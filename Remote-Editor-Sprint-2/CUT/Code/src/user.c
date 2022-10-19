/********************************************
 * *FILENAME	      : server.c
 *
 * *DESCRITION        : This file defines the functions that are performed from user end. 
 * 			            
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
#include <stdio.h>

/*user defined headers*/
#include <user.h>

#define DIR "./data/home/"
/********************************************
 * *FUNCTION NAME : User
 *
 * *DESCRIPTION   : This function is responsible to initialise the name , password and
 *                   current directory of user inside the user structure.
 *
 *
 * 
 *
 *
 *
*********************************************/
void User(const char *name, const char *password, user *u) {
    char dir[100] = "";
    strcpy(dir, DIR);
    strcat(dir, name);
    strcpy(u->dir, dir);
    strcpy(u->name, name);
    strcpy(u->password, password);
}
/********************************************
 * *FUNCTION NAME : ChangeUserDir
 *
 * *DESCRIPTION   : This function is responsible to change the directory of user to the new directory.
 *
 *
 * *RETURNS       : return 0
 *
 *
 *
*********************************************/
int ChangeUserDir(user *u, const char *dir) {
    if (strcmp(dir, "") == 0) {
        strcpy(u->dir, DIR);
        strcat(u->dir, u->name);
    } else
        strcpy(u->dir, dir);
    return 0;
}
