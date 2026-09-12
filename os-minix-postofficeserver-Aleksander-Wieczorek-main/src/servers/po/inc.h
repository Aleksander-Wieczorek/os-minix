#define _POSIX_SOURCE      1	/* tell headers to include POSIX stuff */
#define _MINIX             1	/* tell headers to include MINIX stuff */
#define _SYSTEM            1    /* get OK and negative error codes */
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/config.h>
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <minix/sysutil.h>
#include <minix/const.h>
#include <minix/type.h>
#include <minix/syslib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <machine/vm.h>
#include <machine/vmparam.h>
#include <sys/vm.h>

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int do_post(message *);
int do_retrieve(message *);
int do_retr_wait(message *);
int do_check(message *);
int do_send_back(message *);
int do_forward(message *);
int do_send_bomb(message *);
int remove_from_waitlist(message *);
void check_bombs(void);
int manage_waiter(int,endpoint_t,vir_bytes,pid_t,int);
void setup_next_alarm(void);
endpoint_t pid_to_endpoint(pid_t);
void manage_box_overflow(void);
void manage_waiter_overflow(void);
