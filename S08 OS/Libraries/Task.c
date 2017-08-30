#include <Task.h>
#include <hidef.h> 

#define MAX_TASKS	10

Task_t task[MAX_TASKS];
unsigned char task_index;
unsigned int **stackPointer;
unsigned char running_task;

void ActivateTask(unsigned char task_id){
	DisableInterrupts;
	SaveContext(running_task);
	__asm ais #2;	/* Reajustamos el stackpointer */
	EnableInterrupts;
}

/*	Para las funciones de SaveContext() y RestoreContext()
 * 
 * Orden del Stack:
 * 		- ProgramCounter LOW
 * 		- ProgramCounter HIGH
 * 		- Index Register  (LOW BYTE X)
 * 		- Acumulator A
 * 		- Condition Code Register	
 * 	*/

static void SaveContext(unsigned char task_id){
	__asm ais #3;	/* Movemos el stack tres posiciones para recuperar */
	__asm tsx;		/* Pasamos lo que hay en el Stack al regitro H:X*/
	__asm ais #3	/* Regresamos el Stack pointer a donde estaba */
	__asm sthx	stackPointer;	/* Cargamos la dirección del stack a la variable en memoria */
	task[task_id].TCB.ProgramCounter = *stackPointer;
}

static void RestoreContext(unsigned char task_id){
	(void)task_id;
}
