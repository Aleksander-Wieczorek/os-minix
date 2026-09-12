#include<po.h>
#define MAX_BOXES 256
#define FREE 0
#define TAKEN 1
#define AWAITED 2
typedef struct {
    package pkg;
    int status;

    pid_t sender_pid;
    endpoint_t owner_endpoint;

    int is_bomb;
    clock_t bomb_deadline;

    vir_bytes pkg_addr; 
} mailbox_t;

extern mailbox_t mailboxes[MAX_BOXES];