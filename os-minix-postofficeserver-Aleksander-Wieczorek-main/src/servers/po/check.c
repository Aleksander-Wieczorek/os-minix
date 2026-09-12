#include "inc.h"
#include "glo.h"

int do_check(message *m)
{
    endpoint_t who_sent = m->m_source;
    pid_t who_asks = getnpid(who_sent);
    
    /* just iterate through array */
    /* if found read info*/
    int slot=who_sent%MAX_BOXES;
    if(mailboxes[slot].owner_endpoint!=who_sent || mailboxes[slot].status==FREE) return ENOMSG;

    if(mailboxes[slot].status == TAKEN){
            m->m1_i1 = mailboxes[slot].sender_pid;
            return OK;
    }

    return ENOMSG;
}