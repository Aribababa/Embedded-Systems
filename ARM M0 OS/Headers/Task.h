#define STACK_SIZE  64
#define TASK_LIMIT 10

#define AUTOSTART       ('A')
#define NO_AUTOSTART    ('N')

/* Contiene todos los estados que puede tomar nustras tareas */
typedef enum{
    TASK_STATE_READY,
    TASK_STATE_WAITING,
    TASK_STATE_RUNNING,
    TASK_STATE_SUSPENDED
} TaskState;

/* Estructura que define todo lo que debe tener el proceso */
typedef struct{
    unsigned char PID;      /* Contiene el número de proceso */
    unsigned char Priority; /* Prioridad del proceso */
    unsigned char Autostart;
    unsigned int *Stack;
    unsigned int LinkRegister;
    TaskState State;
} Task_t;

extern Task_t Task[TASK_LIMIT];
extern unsigned char current_task;
unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];

void CreateTask(unsigned char task_id, void (*task_function)(void), unsigned char priority, unsigned char autostart);

void ActivateTask(unsigned char task_id);

void ChainTask(unsigned char task_id);

void TerminateTask(void);

unsigned int __get_LR(void);

void __set_LR(unsigned int LinkRegister);


