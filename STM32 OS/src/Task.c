#include "Task.h"

#define XPSR_DEFAULT 0x01000000

/* Tabla de procesos existentes en el sistema */
task_table_t m_task_table;

/* Procesos ejecutandice en el sistema */
volatile task_t *os_curr_task;
volatile task_t *os_next_task;

/* Arreglo con los Stacks de cada proceso */
volatile os_stack_t ProcessStack[MAX_TASKS][STACK_SIZE];


unsigned char CreateTask(uint8_t Task_ID, uint8_t Priority, task_start_t AutoStart ,void (*task)(void)){
	/* Primero verificamos si aun hay espacio para nuevos procesos, en caso que no regresa zero */
	if (m_task_table.size >= MAX_TASKS-1)
		return 0;

	/* Iniciamos la estructura de la tarea y colocamos el Stackpointer hasta arriba
	  menos 16 registros para dejar espacio para los regidtros ya definidos */
	task_t *p_task = &m_task_table.tasks[m_task_table.size];
	/* Indicamos la tare aque ejecuttará este proceso */
	p_task->handler = task;
	/* Le damosla direccion del Stack que le toa al proceso */
	p_task->sp = (uint32_t)(ProcessStack[m_task_table.size]+STACK_SIZE-16);
	/* Asignamos la prioridad del proceso */
	p_task->priority = Priority;
	/* Verificamos si el proceso iniciará automáticamente */
	p_task->status = (AutoStart == TASK_AUTOSTART) ? OS_TASK_STATUS_READY : OS_TASK_STATUS_SUSPENDED;
	/*Asignamos el número de proceso */
	p_task->PID = Task_ID;


	/* Guardmos los registros especiales:
	 	 - XPSR: Flags de los procesos. Por default es 0x01000000.
	 	 - PC: Apunta hacia la función que ejecutará.
	 	 - LR: Apunta a la funcion que será llamada cuanto la tarea termine.*/
	ProcessStack[m_task_table.size][STACK_SIZE-1] = XPSR_DEFAULT;
	ProcessStack[m_task_table.size][STACK_SIZE-2] = (uint32_t)task;
	ProcessStack[m_task_table.size][STACK_SIZE-3] = (uint32_t) &task_finished;

	/* Aumentamos para dar paso al siguiente proceso */
	m_task_table.size++;

	return 1;
}

void ActivateTask(uint8_t Task_ID){
	__disable_irq();
	for(uint8_t i = 0; i < m_task_table.size; i++){
		if(m_task_table.tasks[i].PID == Task_ID){
			m_task_table.tasks[i].status = OS_TASK_STATUS_READY;
		}
	}
	__enable_irq();
}

void TerminateTask(void){
	__disable_irq();
	os_curr_task->status = OS_TASK_STATUS_SUSPENDED;
	__enable_irq();
}

/* Tarea a la que se llega cuando se termina el proceso */
void task_finished(void){
	volatile uint32_t i = 0;
	while (1){
		i++;
	}
}
