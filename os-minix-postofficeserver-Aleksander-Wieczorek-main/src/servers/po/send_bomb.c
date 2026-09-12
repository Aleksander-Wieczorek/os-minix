#include "inc.h"
#include "glo.h"

int do_send_bomb(message *m) {
    /* work just like normal post, but with bomb setup  */
    pid_t target_pid = m->m1_i1;
    endpoint_t target_e = pid_to_endpoint(target_pid);

    pid_t sender_pid = getnpid(m->m_source);
    int timer_us = m->m1_i2;
    vir_bytes user_pkg_addr=0;
    endpoint_t sender_e = m->m_source;

    int slot = target_e%MAX_BOXES;
    int handled = manage_waiter(slot, 
                                sender_e, user_pkg_addr, 
                                sender_pid,1);

    if (handled == 1)
        return OK;

    if (handled < 0)
        return handled;

    int is_busy=OK;
    if(mailboxes[slot].owner_endpoint==target_e 
        && mailboxes[slot].status==TAKEN) 
    {
        is_busy=EBUSY;
        target_pid = getnpid(sender_e);
        target_e = sender_e;
        slot = sender_e%MAX_BOXES;
        if(mailboxes[slot].owner_endpoint==target_e 
            && mailboxes[slot].status==TAKEN) 
        {
            sys_kill(sender_e, SIGTERM);
            return EBUSY;
        }
    };

    mailboxes[slot].owner_endpoint = target_e;
    mailboxes[slot].sender_pid = sender_pid; 
    mailboxes[slot].is_bomb = 1;
    mailboxes[slot].status = TAKEN;
    clock_t now;
    getuptime(&now);
    clock_t ticks = (timer_us*sys_hz()) /1000000 ;
    mailboxes[slot].bomb_deadline = now + ticks;
    setup_next_alarm();
    return is_busy;
}