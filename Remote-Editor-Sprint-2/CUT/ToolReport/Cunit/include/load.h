#ifndef LOAD_H_
#define Load_H_
#define SIZE 500
typedef struct User {
    char name[100];
    char password[100];
    //char dir[100];
} user;
typedef struct Server {
    user users[100];
    int n;
} ser;

int LoadUsersData(ser *ser, char *filename);
#endif /* load_H_ */