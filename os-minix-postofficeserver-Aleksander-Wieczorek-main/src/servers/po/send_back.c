#include "inc.h"
#include "glo.h"

int do_send_back(message *m) {
    endpoint_t sender_e = m->m_source;
    pid_t sender_pid = getnpid(sender_e);
    
    /* iterate to find message to move */
    int slot = sender_e%MAX_BOXES;
    
    if(mailboxes[slot].owner_endpoint!=sender_e 
        || mailboxes[slot].status==FREE) return ENOMSG;
    pid_t target_pid = mailboxes[slot].sender_pid;
    endpoint_t target_e = pid_to_endpoint(target_pid);
    int target_slot = target_e%MAX_BOXES;
    /* could possibly wake waiter*/
    int handled = manage_waiter(target_slot,
                        SELF,
                        (vir_bytes)&mailboxes[slot].pkg,
                        mailboxes[slot].sender_pid,
                        mailboxes[slot].is_bomb);

    if (handled == 1) {
        mailboxes[slot].status = FREE;
        return OK;
    }

    if (handled < 0)
        return handled;
    if(mailboxes[target_slot].owner_endpoint==target_e 
        && mailboxes[target_slot].status==TAKEN) return EBUSY;

    mailboxes[target_slot]=mailboxes[slot];
    mailboxes[target_slot].owner_endpoint=target_e;

    mailboxes[slot].status=FREE;

    /* if message is given, remove it from array*/
    
    return OK;
}