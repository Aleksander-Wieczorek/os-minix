#include <lib.h>
#include <po.h>
#include <unistd.h>
#include <minix/rs.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
/*
I think each function should be in different files
But, keeping them in one file makes it easier work
*/
int post(package *pp, pid_t pid) {
    if (pp == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (pid <= 0) {
        errno = ESRCH;
        return -1;
    }
    if (kill(pid, 0) != 0)
    {
        //errno=ESRCH;
        return -1;
    }
    endpoint_t po_ep;
    message m;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));
    m.m1_i1 = pid;
    m.m1_p1 = (char *)pp;
    
    return _syscall(po_ep, PO_POST, &m);
}
int retrieve(package *pp, pid_t *pidp) {
    if(pp==NULL){
        errno=EINVAL;
        return -1;
    }
    endpoint_t po_ep;
    message m;
    int r;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));

    m.m1_p1 = (char *)pp;
    r = _syscall(po_ep, PO_RETRIEVE, &m);
    if (r == 0 && pidp != NULL) {
        *pidp = m.m1_i1;
    }
    return r;
}
int check(pid_t *pidp) {
    endpoint_t po_ep;
    message m;
    int r;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));
    r = _syscall(po_ep, PO_CHECK, &m);
    if (r == 0 && pidp != NULL) {
        *pidp = m.m1_i1;
    }
    return r;
}
int retrieve_wait(package *pp, pid_t *pidp){
    if(pp==NULL){
        errno=EINVAL;
        return -1;
    }
    endpoint_t po_ep;
    message m;
    int r;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));

    m.m1_p1 = (char *)pp;
    r = _syscall(po_ep, PO_RETR_WAIT, &m);
    if (r == 0 && pidp != NULL) {
        *pidp = m.m1_i1;
    }
    return r;
}
int send_back() {
    endpoint_t po_ep;
    message m;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));
    
    return _syscall(po_ep, PO_SEND_BACK, &m);
}
int forward(pid_t pid) {
    if (pid <= 0) {
        errno = ESRCH;
        return -1;
    }
    if (kill(pid, 0) != 0)
    {
        errno=ESRCH;
        return -1;
    }
    endpoint_t po_ep;
    message m;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));
    m.m1_i1 = pid;
    return _syscall(po_ep, PO_FORWARD, &m);
}
int send_bomb(pid_t pid, int timer) {
    pid_t self=getpid();
    if(timer<=0){
        kill(self,SIGTERM);
        return -1;
    }
    if (pid <= 0) {
        errno = ESRCH;
        return -1;
    }
    if (kill(pid, 0) != 0)
    {
        //errno=ESRCH;
        return -1;
    }
    endpoint_t po_ep;
    message m;
    if (minix_rs_lookup("po", &po_ep) != 0) {
        return ENOSYS;
    }
    memset(&m, 0, sizeof(m));
    m.m1_i1 = pid;
    int r = _syscall(po_ep, PO_BOMB, &m);
    return r;
}