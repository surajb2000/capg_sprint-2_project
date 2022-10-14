/* This is the user class's implementation of member functions. */

#include "./include/user.h"
#include <stdio.h>

#define DIR "./data/home/"

/*
 * Constructor for the user class.
 */
void User(const char *name, const char *password,user *u) {
    char dir[100] = "";
    strcpy(dir, DIR);
    strcat(dir, name);
    strcpy(u->dir,dir);
    strcpy(u->name,name);
    strcpy(u->password,password);
}

/*
 * Returns the name of the user.
 */
char *GetName(user *u) {
    return u->name;
}

/*
 * changes the directory of the user.
 */
int ChangeUserDir(user *u, const char *dir) {
    if (strcmp(dir, "")) {
        strcpy(u->dir, DIR);
        strcat(u->dir, u->name);
    } else
        strcpy(u->dir,dir);
    return 0;
}

/*
 * Returns the directory of the user.
 */
// char *GetDir(user *u) {
//     return u->dir;
// }

// /*
//  * overloading == operator to compare two users.
//  */
// bool User::operator==(const user *u) {
//     return (this->name == u.name && this->password == u.password);
// }
