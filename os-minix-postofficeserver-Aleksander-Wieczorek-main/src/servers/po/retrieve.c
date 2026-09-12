#include "inc.h"
#include "glo.h"

int do_retrieve(message *m) {
    endpoint_t user_e = m->m_source;
    pid_t user_pid = getnpid(m->m_source);
    vir_bytes user_pkg_addr = (vir_bytes) m->m1_p1;
    int slot=user_e%MAX_BOXES;
    /*check for pending message*/
    if(mailboxes[slot].owner_endpoint!=user_e || mailboxes[slot].status==FREE) return ENOMSG;
    if (mailboxes[slot].is_bomb) {
            // KABOOM
            sys_kill(user_e, SIGTERM);
            mailboxes[slot].status = TAKEN;
            return EDONTREPLY;
    }

    int r = sys_datacopy(SELF, (vir_bytes)&mailboxes[slot].pkg, 
                        user_e, user_pkg_addr,
                        sizeof(package));
    
    if (r != OK) return EINVAL;

    m->m1_i1=mailboxes[slot].sender_pid;
    mailboxes[slot].status=FREE;
    return OK;
}