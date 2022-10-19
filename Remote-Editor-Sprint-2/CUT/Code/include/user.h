#ifndef USER_H
#define USER_H

#include <string.h>
#define MAX_STRING_SIZE 100

typedef struct User {
    char name[MAX_STRING_SIZE];
    char password[MAX_STRING_SIZE];
    char dir[MAX_STRING_SIZE];
} user;

void User(const char *name, const char *password,user *);
int ChangeUserDir(user *u,const char *new_dir);

#endif
