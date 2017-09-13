#include "Task.h"
#include "OSv2.h"
#include "derivative.h"
#include <hidef.h> 

Task_t task[MAX_TASKS];	/* Arreglo de tareas disponibles en el sistema operaivo */
unsigned char Acc;
unsigned int **stackPointer;
unsigned int *pc_actual;
unsigned char running_task;

DeclareTask(ROOT);

void CreateTask(unsigned char task_id, unsigned char priority, unsigned char autostart, void *(task_function)()){
	/* Creamos el proceso ROOT, un proceso que corre cuando el sistema operativo no tiene tareas que realizar */
	if(1 == 1){		/* TODO: Hay que arreglar esto ... */
		task[0].Task_id = 0;
		task[0].Autostart = AUTOSTART;
		task[0].Priority = 0;
		task[0].TCB.ProgramCounter = &ROOT;
		task[0].State = READY;
	}
	task[task_id].Task_id = (task_id > 0x01) ? task_id : 0x01;	/* Verificamos que no se pueda escribir nada en la tarea cero */
	task[task_id].Priority = ++priority;	/* No dejmos que la prioridad sea cero */
	task[task_id].Autostart = autostart;
	task[task_id].State = SUSPENDED;		/* Siempre debe haber alguien que active la tarea, por eso esta en SUSPENDD */
	task[task_id].TCB.ProgramCounter = task_function;
}

extern void ActivateTask(unsigned char task_id){
	DisableInterrupts;
	SaveContext();
	task[running_task].State = READY;	/* El proceso que estaba corriendo para a READY */
	task[task_id].State = READY;		/* El proceso que se activo pasa a READY */
	__asm ais #2;
	EnableInterrupts;
	(void)Schedule();					/* No hay necesidad de poner el void, pero tampoco le queda mal :P */
}

extern void ActivateTask_ISR(unsigned char task_id){
	DisableInterrupts;
	SaveContextISR();
	task[running_task].Interrupted = 1;
	task[running_task].State = READY;
	task[task_id].State = READY;	
	__asm ais #2;
	EnableInterrupts;
	(void)Schedule();
}

extern void ActivateTask_Alarm(unsigned char task_id){
	task[task_id].State = READY;	/* Solo ponemos la tarea en READY */
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
	EnableInterrupts;
	if(running_task){	/* Verificamos si el proceso no es el proceso ROOT */
		task[running_task].State = SUSPENDED;
		__asm ais #3;
	}
	__asm AIS #2;
	DisableInterrupts;
	/* Este Warning es causado por la instruccion de ensamblador dentro del IF. No afecta al sistema operativo,
	 * pero el IDE lo marca ya que puede haber un desfase en el OS. NO HAY DSFASE. */
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
	/* Funciona por lo pronto, Después averiguamos como hacerlo de manera más eficiente */
	__asm ais #3;	/* Movemos el stack tres posiciones para recuperar */
	__asm tsx;		/* Pasamos lo que hay en el Stack al regitro H:X*/
	__asm sthx	stackPointer;	/* Cargamos la dirección del stack a la variable en memoria */
	__asm ais #-3;
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
	/* Funciona por lo pronto, Después averiguamos como hacerlo de manera más eficiente */
	/* Sacamos los datos del stack de la interrupcion */
	__asm LDA 6, SP;
	__asm STA	Acc;
	task[running_task].TCB.Interrupt[4] = Acc;
	__asm LDA 7, SP;
	__asm STA	Acc;
	task[running_task].TCB.Interrupt[3] = Acc;
	__asm LDA 8, SP;
	__asm STA	Acc;
	task[running_task].TCB.Interrupt[2] = Acc;
	__asm LDA 9, SP;
	__asm STA	Acc;
	task[running_task].TCB.Interrupt[1] = Acc;
	__asm LDA 10, SP;
	__asm STA	Acc;
	task[running_task].TCB.Interrupt[0] = Acc;
	
	
	/* Comenzamos a guardar las variables de la tarea */
	__asm LDA	11,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[5] = Acc;
	__asm LDA	12,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[4] = Acc;
	__asm LDA	13,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[3] = Acc;
	__asm LDA	14,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[2] = Acc;
	__asm LDA	15,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[1] = Acc;
	__asm LDA	16,SP
	__asm STA	Acc;
	task[running_task].TCB.Stack[0] = Acc;
}

void RestoreContext(void){
	/* Funciona por lo pronto, Después averiguamos como hacerlo de manera más eficiente */
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

void RestoreContext_ISR(void){
	/* Funciona por lo pronto, Después averiguamos como hacerlo de manera más eficiente */
	/* Cargamos las variables del proceso en el StackPointer */
	__asm ais #14;
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

	/* Cargamos los datos que cargó la interrupcion en el Stack */
	loader = task[running_task].TCB.Interrupt[0];
	__asm LDA loader;
	__asm PSHA;
	loader = task[running_task].TCB.Interrupt[1];
	__asm LDA loader;
	__asm PSHA;
	loader = task[running_task].TCB.Interrupt[2];
	__asm LDA loader;
	__asm PSHA;
	loader = task[running_task].TCB.Interrupt[3];
	__asm LDA loader;
	__asm PSHA;
	loader = task[running_task].TCB.Interrupt[4];
	__asm LDA loader;
	__asm PSHA;
	__asm ais #1;
	
	EnableInterrupts;
	__asm RTI;
}

/* Aqui se euncuentra el proceso que corre cuando el sistema operativo no tiene tareas que hacer.
 * No hay que crear variables, ya que esta tarea no puede almacennarlas.  */
TASK(ROOT){
	EnableInterrupts;	/* Por si acaso lo colocamos, no esta demas prevenir que se bloqueen las interrupciones */
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
