#include "inc.h"
#include "glo.h"

int do_post(message *m) {
    /* setup package. If someone waits - give it to him  */
    /* else add package to free space  */

    pid_t target_pid = m->m1_i1;
    pid_t sender_pid = getnpid(m->m_source);
    vir_bytes user_pkg_addr = (vir_bytes) m->m1_p1;
    endpoint_t sender_e = m->m_source;
    endpoint_t owner_e = pid_to_endpoint(target_pid);
    if (user_pkg_addr == 0) return EINVAL;
    
    //int r = manage_waiter(target_pid, user_pkg_addr, sender_e, sender_pid,0);
    //if(r!=ESRCH) return r;

    int slot = owner_e%MAX_BOXES;
    if(mailboxes[slot].owner_endpoint==owner_e 
        && mailboxes[slot].status==TAKEN) return EBUSY;


    int handled = manage_waiter(slot, sender_e, user_pkg_addr, sender_pid,0);

    if (handled == 1)
        return OK;

    if (handled < 0)
        return handled;

    int r = sys_vircopy(sender_e, user_pkg_addr, 
                        SELF, (vir_bytes)&mailboxes[slot].pkg, 
                        sizeof(package));
    
    if (r != OK) return EINVAL;
    mailboxes[slot].status = TAKEN;
    mailboxes[slot].owner_endpoint = owner_e;
    mailboxes[slot].sender_pid = sender_pid; 
    mailboxes[slot].is_bomb=0;
    return OK;
}