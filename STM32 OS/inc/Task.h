#include "stm32f4xx.h"

#define MAX_TASKS 10
#define STACK_SIZE	32

/* Definimos el tipo de dato para el stack de los proceso */
typedef uint32_t os_stack_t;

/* Estados que pueden tener las tareas */
typedef enum {
	OS_TASK_STATUS_READY = 1,
	OS_TASK_STATUS_RUNNING,
	OS_TASK_STATUS_WAITING,
	OS_TASK_STATUS_SUSPENDED
} task_status_t;

typedef enum {
	TASK_AUTOSTART = 1,
	TASK_NO_AUTOSTART
}task_start_t;

/* Estructura de las tareas que tiene el sistema operativo */
typedef struct {
	/* La estructura guarda al direccion del Stackpointer para ficilitar el cambio de contexto */
	volatile os_stack_t sp;
	void (*handler)(void);
	volatile task_status_t status;
	unsigned char priority;
	unsigned char PID;
} task_t;

/* Tabla de procesos en el sistema */
typedef struct {
	task_t tasks[MAX_TASKS];	/* Contiene todas las tareas del sistema */
	volatile uint32_t current_task;
	uint32_t size;
} task_table_t;

/* Variables externas */
extern task_table_t m_task_table;
extern volatile task_t *os_curr_task;
extern volatile task_t *os_next_task;

/* Funciones del sistema operativo */

unsigned char CreateTask(uint8_t Task_ID, uint8_t Priority, task_start_t AutoStart ,void (*task)(void));

void ActivateTask(uint8_t Task_ID);

void TerminateTask(void);

void task_finished(void);
