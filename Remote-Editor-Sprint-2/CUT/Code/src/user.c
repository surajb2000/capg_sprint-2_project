#include <stdio.h>

/*user defined headers*/
#include <user.h>

#define DIR "./data/home/"


void User(const char *name, const char *password, user *u) {
    char dir[100] = "";
    strcpy(dir, DIR);
    strcat(dir, name);
    strcpy(u->dir, dir);
    strcpy(u->name, name);
    strcpy(u->password, password);
}

int ChangeUserDir(user *u, const char *dir) {
    if (strcmp(dir, "") == 0) {
        strcpy(u->dir, DIR);
        strcat(u->dir, u->name);
    } else
        strcpy(u->dir, dir);
    return 0;
}
