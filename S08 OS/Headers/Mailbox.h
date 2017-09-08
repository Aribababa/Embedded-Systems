#define Semaphore_t	(Mailbox_t)

typedef struct {
	unsigned char Mailbox_id;
	unsigned char Consumer_id;
	unsigned char Productor_id;
	unsigned char Message;
	unsigned char Flag;
} Mailbox_t;


void CreateMailbox(unsigned char mailbox_id, unsigned char consumer_id, unsigned char producer_id);

void SetMail(unsigned char mailbox_id, unsigned char message);

unsigned char GetMail(unsigned char mailbox_id);

