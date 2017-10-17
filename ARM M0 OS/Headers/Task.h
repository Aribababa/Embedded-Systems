#define STACK_SIZE  16
#define TASK_LIMIT 16

#define AUTOSTART       ('A')
#define NO_AUTOSTART    ('N')

/* Contiene todos los estados que puede tomar nustras tareas */
typedef enum{
    TASK_STATE_READY,
    TASK_STATE_WAITING,
    TASK_STATE_RUNNING,
    TASK_STATE_SUSPENDED
} TaskState;

typedef struct{
    unsigned int registers[8];  /* Para los registros del r0 al r7 */
    unsigned int lr;
} TaskControlBlock;

/* Estructura que define todo lo que debe tener el proceso */
typedef struct{
    unsigned char PID;      /* Contiene el número de proceso */
    unsigned char Priority; /* Prioridad del proceso */
    unsigned char Autostart;
    unsigned int *Stack;
    unsigned int LinkRegister;  /* Guarda el punto donde se quedo la tarea */
    unsigned int initAddress;   /* Guarda cual es el punto inicial de la tarea */
    TaskControlBlock TCB;
    TaskState State;
} Task_t;

extern Task_t Task[TASK_LIMIT];
extern unsigned char current_task;
extern unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];
extern unsigned char Interrupted;

void CreateTask(unsigned char task_id, void (*task_function)(void), unsigned char priority, unsigned char autostart);

void ActivateTask(unsigned char task_id);

void ActivateTask_ISR(unsigned char task_id);

void ChainTask(unsigned char task_id);

void TerminateTask(void);



