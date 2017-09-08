#include <Mailbox.h>
#include "Task.h"
#include "OSv2.h"
#define MAILBOXES	10

Mailbox_t semaphore[MAILBOXES];

void CreateMailbox(unsigned char mailbox_id, unsigned char consumer_id, unsigned char producer_id){
	semaphore[mailbox_id].Mailbox_id = mailbox_id;
	semaphore[mailbox_id].Consumer_id = (consumer_id >= 1 && consumer_id < 10) ? consumer_id : 0x01;
	semaphore[mailbox_id].Productor_id = (producer_id > 0 && producer_id < 10 && producer_id != consumer_id) ? producer_id : 0x02;
	semaphore[mailbox_id].Flag = 0x00;	/* No hay mensaje por default */
}
