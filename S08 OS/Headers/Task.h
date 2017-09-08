/* Algunas definiciones para facilitar el c�digo */
#define AUTOSTART 		1
#define NO_AUTOSTART	0

/* N�mero de tareas que permite el sistema operativo */
#define MAX_TASKS	10

/* Definimos los estados que tendr�n los procesos */
#define SUSPENDED	('S')
#define WAITING		('W')
#define RUNNING		('R')
#define READY		('L')

/* Definimos la estructura para guardar los registros utilizados por loas taeras*/
typedef struct{
	unsigned char CCR;
	unsigned char Acumulator;
	unsigned char IndexRegister;
	unsigned char Stack[6];
	void *ProgramCounter;
} TaskControlBlock;

/* Estructura definida para las tareas del sistema operativo */
typedef struct{
	unsigned char Task_id;
	unsigned char Priority;
	unsigned char Autostart;
	unsigned char State;
	unsigned char Interrupted;
	TaskControlBlock TCB;
} Task_t;

/* Variables que se tienen que compartir con otros archivos */
extern Task_t task[MAX_TASKS];
extern unsigned int temporal;
extern unsigned char running_task;

extern void CreateTask(unsigned char task_id, unsigned char priority, unsigned char autostart, void *(task_function)());

extern void ActivateTask(unsigned char task_id);

extern void ActivateTask_Alarm(unsigned char task_id);
	/* Hace lo mismo que ActivateTask(), solo que esta funcion no llama al Scheduler */

extern void ChainTask(unsigned char task_id);

extern void TerminateTask(void);

static void SaveContext(void);

static void SaveContextISR(void);
		
void RestoreContext(void);

void SaveStackPointer(void);
	/* Guarda el valor del Stack pointer y lo guarda en el Task Contrl Block del proceso que
	 * solicit� la funci�n */
