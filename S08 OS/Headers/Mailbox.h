#define Semaphore_t	(Mailbox_t)

typedef struct {
	unsigned char Mailbox_id;	/* Identificador del Mailbox */
	unsigned char Consumer_id;
	unsigned char Productor_id;
	unsigned char Message;
	unsigned char Flag;
} Mailbox_t;


void CreateMailbox(unsigned char mailbox_id, unsigned char consumer_id, unsigned char producer_id);


void SetMail(unsigned char mailbox_id, unsigned char message);
	/* Coloca un dato en el buzon y activa la bandera. Solo si es el prductor correcto puede colcar un dato 
	 * en el mailbox. */

unsigned char WaitMessage(unsigned char id_mailbox);
	/* Coloca el proceso en estado de WAIT y salta al Scheduler. Pone en cola la tarea. Este regresa 1 si 
	 * la Flag del semáforo esta activa, en caso constrario regresa 0. */

unsigned char GetMail(unsigned char mailbox_id);


