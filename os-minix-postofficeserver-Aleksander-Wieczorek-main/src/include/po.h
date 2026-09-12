#ifndef POSTOFFICE_H
#define POSTOFFICE_H
#define PO_POST	1
#define PO_RETRIEVE	2
#define PO_RETR_WAIT   3
#define PO_CHECK   4
#define PO_SEND_BACK   5
#define PO_FORWARD 6
#define PO_BOMB    7
#include <sys/types.h>

typedef struct {
    char data[128];
} package;

int post(package *pp, pid_t pid);
int retrieve(package *pp, pid_t *pidp);
int retrieve_wait(package *pp, pid_t *pidp);
int check(pid_t *pidp);
int send_back(void);
int forward(pid_t pid);
int send_bomb(pid_t pid, int timer);

#endif /* POSTOFFICE_H */