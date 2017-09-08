#include <Mailbox.h>
#include "Task.h"
#include "OSv2.h"
#define MAILBOXES	10

Mailbox_t semaphore[MAILBOXES];

voidCreateMailbox(unsigned char mailbox_id, unsigned char consumer_id, unsigned char producer_id){
	semaphore[mailbox_id].Mailbox_id = mailbox_id;
	semaphore[mailbox_id].Consumer_id = (consumer_id >= 1 && consumer_id < 10) ? consumer_id : 0x01;
	semaphore[mailbox_id].Productor_id = (producer_id > 0 && producer_id < 10 && producer_id != consumer_id) ? producer_id : 0x02;
	semaphore[mailbox_id].Flag = 0x00;	/* No hay mensaje por default */
}

void SetMail(unsigned char mailbox_id, unsigned char message){
	if(semaphore[mailbox_id].Flag == 0x00){
		if(semaphore[mailbox_id].Productor_id == running_task){
			semaphore[mailbox_id].Message = message;
			semaphore[mailbox_id].Flag = 0x01;
		}
	}
}

unsigned char GetMail(unsigned char mailbox_id){
	if(semaphore[mailbox_id].Flag == 0x01){
		if(semaphore[mailbox_id].Consumer_id == running_task){
			semaphore[mailbox_id].Flag = 0x00;
			return message;
		}
	}
	return 0;
}
