#include "inc.h"
#include "glo.h"
#include <minix/com.h>

int manage_waiter(int slot,
                    endpoint_t src_e,
                    vir_bytes src_addr,
                    pid_t sender_pid,
                    int bomb)
{
    if (mailboxes[slot].status != AWAITED)
        return 0;

    endpoint_t waiter_e = mailboxes[slot].owner_endpoint;

    if (bomb) {
        sys_kill(waiter_e, SIGTERM);
        message reply;
        reply.m_type = EINTR;
        mailboxes[slot].status = FREE;
        mailboxes[slot].is_bomb = 0;
        send(waiter_e, &reply);
        return 1;
    }

    vir_bytes waiter_buf = mailboxes[slot].pkg_addr;

    int r = sys_vircopy(src_e, src_addr,
                        waiter_e, waiter_buf,
                        sizeof(package));

    if (r != OK)
        return r;

    message reply;
    reply.m_type = OK;
    reply.m1_i1 = sender_pid;
    mailboxes[slot].status = FREE;
    send(waiter_e, &reply);

    return 1;
}
void check_bombs()
{
    /* alarm said that some bomb could went off - check them all*/
    clock_t now;
    getuptime(&now);
    for (int i = 0; i < MAX_BOXES; i++) {
        if (mailboxes[i].is_bomb && now>=mailboxes[i].bomb_deadline && mailboxes[i].status == TAKEN) {
            sys_kill(mailboxes[i].owner_endpoint, SIGTERM);
            mailboxes[i].status = FREE;
        }
    }
}

endpoint_t pid_to_endpoint(pid_t pid) {
    /* simple translator pid->endpoint*/
    message m;
    int r;

    m.m_type = GETPROCNR; 
    m.m1_i1 = pid;         
    m.m1_i2 = 0;          

    r = sendrec(PM_PROC_NR, &m);


    if (m.m_type != OK) {
        return -1; 
    }

    return m.m1_i1;
}


void setup_next_alarm()
{
    /* alarm unfortunately don't stack*/
    /* if time to next explosion COULD change reset timer */
    clock_t best = 0;
    for(int i=0;i<MAX_BOXES;i++) {
        if(mailboxes[i].is_bomb && mailboxes[i].status == TAKEN) {
            if(best==0 || mailboxes[i].bomb_deadline < best)
                best = mailboxes[i].bomb_deadline;
        }
    }
    if(best != 0)
    {
        sys_setalarm(best, 1);
    }
}

int remove_from_waitlist(message *m) {
    /* signal could wake up someone waiting*/
    pid_t pid = m->m1_i1;
    endpoint_t ep = pid_to_endpoint(pid);
    int slot = ep%MAX_BOXES;
    if(mailboxes[slot].owner_endpoint == ep)
    {
        mailboxes[slot].status = FREE;
        message reply;
        memset(&reply, 0, sizeof(reply));
        reply.m_type = EINTR;
        send(mailboxes[slot].owner_endpoint, &reply);
    }
    return EDONTREPLY;
}