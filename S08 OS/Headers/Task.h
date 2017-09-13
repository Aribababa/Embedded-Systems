/* Algunas definiciones para facilitar el código */
#define AUTOSTART 		1
#define NO_AUTOSTART	0

/* Número de tareas que permite el sistema operativo */
#define MAX_TASKS	10

/* Definimos los estados que tendrán los procesos */
#define SUSPENDED	('S')
#define WAITING		('W')
#define RUNNING		('R')
#define READY		('L')

/* Definimos la estructura para guardar los registros utilizados por loas taeras*/
typedef struct{
	unsigned char Interrupt[5];	/* Arreglo exclusivo para los datos guardados en el stack por las interupciones */
	unsigned char Stack[6];		/* Arreglo plara las variables de las tareas */
	void *ProgramCounter;
} TaskControlBlock;

/* Estructura definida para las tareas del sistema operativo */
typedef struct{
	unsigned char Task_id;		/* Guardamos el número de la tarea */
	unsigned char Priority;		/* Guardamos su prioridad */
	unsigned char Autostart;	/* Flag que indica si automáticamente se iniciará al arranque del OS */
	unsigned char State;		/* Guarda el estado en el que se encuenra la tarea */
	unsigned char Interrupted;	/* Flag que indica si la tarea fue interrumpida */
	TaskControlBlock TCB;		/* Estructura que guarda las variables de la tarea y su Instruction Pointer */
} Task_t;

/* Variables que se tienen que compartir con otros archivos */
extern Task_t task[MAX_TASKS];
extern unsigned int temporal;
extern unsigned char running_task;

extern void CreateTask(unsigned char task_id, unsigned char priority, unsigned char autostart, void *(task_function)());

extern void ActivateTask(unsigned char task_id);

extern void ActivateTask_ISR(unsigned char task_id);

extern void ActivateTask_Alarm(unsigned char task_id);
	/* Hace lo mismo que ActivateTask(), solo que esta funcion no llama al Scheduler */

extern void ChainTask(unsigned char task_id);

extern void TerminateTask(void);

static void SaveContext(void);

static void SaveContextISR(void);
		
void RestoreContext(void);

void RestoreContext_ISR(void);

void SaveStackPointer(void);
	/* Guarda el valor del Stack pointer y lo guarda en el Task Contrl Block del proceso que
	 * solicitó la función */
