#include "inc.h"
#include "glo.h"

int do_retr_wait(message *m) {
    /* work just like retrieve*/
    /* if no message subscribe to waitlist*/
    endpoint_t user_e = m->m_source;
    pid_t user_pid = getnpid(m->m_source);
    vir_bytes user_pkg_addr = (vir_bytes) m->m1_p1;
    if (user_pkg_addr == 0) return EINVAL;

    char dummy_byte;
    int check = sys_datacopy(user_e, user_pkg_addr, 
                            SELF, (vir_bytes)&dummy_byte, 
                            1);
    
    if (check != OK) {
        return EINVAL;
    }
    
    int slot=user_e%256;
    // if you have a message, get it
    if(mailboxes[slot].owner_endpoint == user_e && mailboxes[slot].status == TAKEN)
    {
        if (mailboxes[slot].is_bomb) {
            sys_kill(mailboxes[slot].owner_endpoint, SIGTERM);
        }
        else {
            int r = sys_datacopy(SELF, (vir_bytes)&mailboxes[slot].pkg, 
                        user_e, user_pkg_addr,
                        sizeof(package));
            if (r != OK) return EINVAL;
            m->m1_i1=mailboxes[slot].sender_pid;
        }
        mailboxes[slot].status = FREE;
        return OK;
    }
    // if not, subscribe to the waitlist
    //manage_waiter_overflow();
    mailboxes[slot].owner_endpoint = user_e;
    mailboxes[slot].status= AWAITED;
    mailboxes[slot].pkg_addr = user_pkg_addr;
    return EDONTREPLY;
    
}