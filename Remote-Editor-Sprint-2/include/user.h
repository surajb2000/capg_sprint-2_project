/*
 * This class is used to store user data.
 */
#ifndef USER_H
#define USER_H

#include <string.h>

typedef struct User {
    char name[100];
    char password[100];
    char dir[100];
} user;

user User(const char *name, const char *password);
int ChangeUserDir(user *u,const char *new_dir);
char *GetDir();
// bool operator==(const User &user);
char *GetName();

#endif
