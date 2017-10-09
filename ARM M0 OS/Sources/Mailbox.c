#include "Mailbox.h"
#include "Task.h"
#include "OS.h"

Mailbox_t mailbox[MAILBOX_LIMIT];

void CreateMailbox(unsigned char mailbox_id, unsigned char consumer_id, unsigned char producer_id){
    if(mailbox_id < MAILBOX_LIMIT){
        mailbox[mailbox_id].Mailbox_id = mailbox_id;
        mailbox[mailbox_id].Consumer_id = consumer_id;
        mailbox[mailbox_id].Productor_id = producer_id;
        mailbox[mailbox_id].Flag = 0;  
    }
    return;
}

void SetMail(unsigned char mailbox_id, unsigned int message){
    mailbox[(mailbox_id < MAILBOX_LIMIT) ? mailbox_id : 0].Message = message;
    return;
}

unsigned char GetMail(unsigned char mailbox_id){
    return mailbox[(mailbox_id < MAILBOX_LIMIT) ? mailbox_id : 0].Message;
}

void WaitMessage(void){
    Task[current_task].State = TASK_STATE_WAITING;
    (void)Schedule();
}

unsigned char CheckFlag(unsigned char mailbox_id){
    return mailbox[(mailbox_id < MAILBOX_LIMIT) ? mailbox_id : 0].Flag;
}