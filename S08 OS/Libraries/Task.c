#include "Task.h"
#include "OSv2.h"
#include "derivative.h"
#include <hidef.h> 

Task_t task[MAX_TASKS];
unsigned char Acc;
unsigned int **stackPointer;
unsigned int *pc_actual;
unsigned char running_task;
unsigned char count;

DeclareTask(ROOT);

void CreateTask(unsigned char task_id, unsigned char priority, unsigned char autostart, void *(task_function)()){
	if(1){		/* Luego arreglamos esto */
		task[0].Task_id = 0;
		task[0].Autostart = AUTOSTART;
		task[0].Priority = 0;
		task[0].TCB.ProgramCounter = &ROOT;
		task[0].State = READY;
	}
	task[task_id].Task_id = (task_id > 0x01) ? task_id : 0x01;	/* erificamos que no se pueda escribir nada en la tarea cero */
	task[task_id].Priority = ++priority;
	task[task_id].Autostart = autostart;
	task[task_id].State = SUSPENDED;
	task[task_id].TCB.ProgramCounter = task_function;
}

extern void ActivateTask(unsigned char task_id){
	DisableInterrupts;
	SaveContext();
	task[task_id].State = READY;
	__asm ais #2;
	EnableInterrupts;
	(void)Schedule();
}

extern void ActivateTask_Alarm(unsigned char task_id){
	task[task_id].State = READY;
}

extern void ChainTask(unsigned char task_id){
	DisableInterrupts;
	task[running_task].State = SUSPENDED;		/* Cambiamos el estado del proceso que se estaba ejecutando */
	pc_actual = task[task_id].TCB.ProgramCounter;

	asm{
		AIS	#2
		LDHX	pc_actual
		CLI		; Habilitamos las innterrupciones 
		JMP	,X
	}	
}

extern void TerminateTask(void){
	if(running_task){
		task[running_task].State = SUSPENDED;
	}
	__asm AIS #2;
	(void)Schedule();
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

static void SaveContext(void){
	__asm ais #3;	/* Movemos el stack tres posiciones para recuperar */
	__asm tsx;		/* Pasamos lo que hay en el Stack al regitro H:X*/
	__asm sthx	stackPointer;	/* Cargamos la dirección del stack a la variable en memoria */
	__asm ais #-3;
	
	/* Guardamos el estado de las flags de la tarea */
	__asm TPA;
	__asm STA	Acc;
	task[running_task].TCB.CCR = Acc;
	task[running_task].TCB.ProgramCounter = *stackPointer;
	
	__asm LDA	6,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[5] = Acc;
	
	__asm LDA	7,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[4] = Acc;

	__asm LDA	8,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[3] = Acc;

	__asm LDA	9,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[2] = Acc;

	__asm LDA	10,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[1] = Acc;

	__asm LDA	11,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[0] = Acc;
}

static void SaveContextISR(void){
	__asm LDA	3,SP;	/* Carga en A las banderas que habia cuando se interrumpio la tarea */
	__asm STA 	Acc;
	task[running_task].TCB.CCR = Acc;
	
	__asm LDA	4,SP;	/* Cargamos lo que habia en A antes de la interrupción */
	__asm STA	Acc;
	task[running_task].TCB.Acumulator = Acc;
	
	__asm LDA	5,SP;	/* Cargamos lo que habia en X antes de la interrupción */
	__asm STA	Acc;
	task[running_task].TCB.IndexRegister = Acc;
	
	__asm LDHX	6,SP;
	
}

void RestoreContext(void){
	/* Hay que opimizar esto para que se pueda hacer mediante un loop */
	__asm ais #7;
	loader = task[running_task].TCB.Stack[0];
	__asm LDA loader;
	__asm PSHA;
	
	loader = task[running_task].TCB.Stack[1];
	__asm LDA loader;
	__asm PSHA;
	
	loader = task[running_task].TCB.Stack[2];
	__asm LDA loader;
	__asm PSHA;
	
	loader = task[running_task].TCB.Stack[3];
	__asm LDA loader;
	__asm PSHA;
	
	loader = task[running_task].TCB.Stack[4];
	__asm LDA loader;
	__asm PSHA;
	
	loader = task[running_task].TCB.Stack[5];
	__asm LDA loader;
	__asm PSHA;

	asm{
		LDHX	next_task
		CLI	; Activamos las interrupciones
		JMP	,X
	}
}

/* Aqui se euncuentra el proceso que corre cuando el sistema operativo no tiene tareas que hacer */
TASK(ROOT){
	/* Hay que poner a pollear este proceso para dar tiempo
	 * al sistema de alarmas interrumpirse */
	asm {
		NOP
		NOP
		NOP
		NOP
		NOP
	}
	TerminateTask();
}
