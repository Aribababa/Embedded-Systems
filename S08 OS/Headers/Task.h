/* Algunas definiciones para facilitar el código */
#define AUTOSTART 		1
#define NO_AUTOSTART	0

/* Definimos los estados que tendrán los procesos */
#define SUSPENDED	('S')
#define WAITING		('W')
#define RUNNING		('R')
#define READY		('L')

/* Facilitamos la creación de Tareas para el usuario */
#define	DeclareTask(x)	void x(void)
#define TASK(x)		void x(void)

/*Facilitamos las interrupciones utilizando macros */


/* Definimos la estructura para guardar los registros utilizados por loas taeras*/
typedef struct{
	unsigned char CCR;
	unsigned char Acumulator;
	unsigned char IndexRegister;
	void *ProgramCounter;
} TaskControlBlock;

/* Estructura definida para las tareas del sistema operativo */
typedef struct{
	unsigned char Task_id;
	unsigned char Priority;
	unsigned char Autostart;
	unsigned char State;
	TaskControlBlock TCB;
} Task_t;

void CreateTask(unsigned char task_id, unsigned char priority, unsigned char autostart, unsigned int *(task)());

void ActivateTask(unsigned char task_id);

void ChainTask(unsigned char task_id);

void TerminateTask(void);

static void SaveContext(unsigned char task_id);

static void RestoreContext(unsigned char task_id);

