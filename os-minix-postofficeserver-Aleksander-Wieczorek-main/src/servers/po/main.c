#include "inc.h"
#include "glo.h"
#include <minix/minlib.h> 
int call_type;
mailbox_t mailboxes[MAX_BOXES];
int main(int argc, char *argv[])
{
    message m;
    int ipc_status;
    int r,call_nr;
    endpoint_t who_e;
    sef_startup();
    for(int i=0;i<MAX_BOXES;i++){
        mailboxes[i].status = FREE;
        mailboxes[i].owner_endpoint = -1;
        mailboxes[i].is_bomb = 0;
    }
    while(TRUE){
        if (sef_receive_status(ANY, &m, &ipc_status) != OK) continue;
        who_e = m.m_source;
        call_nr = m.m_type;
        call_type = m.m_type;
        if (is_ipc_notify(ipc_status)) {
            if (who_e == CLOCK) {
                check_bombs();
            }
            
            continue;
        }
        switch(call_type){
            case PO_POST:
                r = do_post(&m);
                break;
            case PO_RETRIEVE:
                r = do_retrieve(&m);
                break;
            case PO_CHECK:
                r = do_check(&m);
                break;
            case PO_RETR_WAIT:
                r = do_retr_wait(&m);
                break;
            case PO_SEND_BACK:
                r = do_send_back(&m);
                break;
            case PO_FORWARD:
                r = do_forward(&m);
                break;
            case PO_BOMB:
                r = do_send_bomb(&m);
                break;
            case EINTR:
                r = remove_from_waitlist(&m);
                break;
            default:
                r = EINVAL;
                //printf("Unknown call type %d\n",call_type);
        }
        if(who_e!= PM_PROC_NR && r!=EDONTREPLY)
        {
            m.m_type = r;
            send(m.m_source,&m);
        }
    }
}