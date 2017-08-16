#include "OS.h"
/* #FueLaPizza */

/* Estados de los Tasks */
#define SUMMIT 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define MAX_TASKS 5

unsigned int running_task = 0;	/* Guarda la posicion del proceso que esta corriendo */
unsigned int **stack_pointer;		/* Guarda la direccion donde se quedo la funcion que se esta corriendo */
unsigned int *pc_actual;

struct Tasks{
	unsigned char priority;
	unsigned char autostart;
	unsigned char state;
	void* initPointer;
	void* continuePointer;
}; 

struct Tasks procesess[MAX_TASKS];
/* Creamos un máximo de 10 procesos */

DeclareTask(ROOT);	
/* Crea el proceso ROOT que corre cuando el sistema operativo no tiene nada que hacer */

void createTask(unsigned char task_id, unsigned char priority ,unsigned char autostart, void *(task)(void)){
	
	if(task_id == 0){
		task_id++;
	}
	
	procesess[task_id].priority = priority+1;
	procesess[task_id].autostart = autostart;
	procesess[task_id].state = SUMMIT;
	procesess[task_id].initPointer = task;
}

void init_OS(void){
	unsigned char i = 0;
	
	/* Creamos la Route Task */
	procesess[0].priority = 0;
	procesess[0].autostart = 1;
	procesess[0].state = READY;
	procesess[0].initPointer = &ROOT;
	procesess[i].continuePointer = procesess[0].initPointer;
	
	do{
		if(procesess[i].state == SUMMIT && procesess[i].autostart){
			procesess[i].continuePointer = procesess[i].initPointer;
			procesess[i].state = READY;
		}
	i++;
	} while(i < MAX_TASKS);
	Schedule();
}

void Schedule(void){
	unsigned char i = 0;
	unsigned char prior = procesess[0].priority;
	unsigned char next_task = 0;
	
	do{
		if(procesess[i].state == READY){
			if(procesess[i].priority >= prior){
				prior = procesess[i].priority;
				next_task = i;
			}
		}		
		i++;
	} while(i < MAX_TASKS);
	running_task = next_task;
	
	pc_actual = procesess[next_task].continuePointer;
	
	asm{
		LDHX	pc_actual
		JMP		,X
	}
}

volatile void Activate_Task(unsigned char Task_id){
	
	asm{
	  nop
	  ais #1 ; Sumale un dato inmediato al Stackpointer
	  tsx     ; Copia el Stackpointer a H:x(Parte alta  en H y baja en X)
	  ais #-1 ; se regresa al stackpointer
	  sthx stack_pointer
	}
	
	procesess[running_task].continuePointer = *stack_pointer;
	
	if(procesess[Task_id].state == SUMMIT){
		procesess[Task_id].state = READY;
		procesess[Task_id].continuePointer = procesess[Task_id].initPointer;
	}
	Schedule();
}

void Chain_Task(unsigned char Task_id){
	procesess[running_task].state = SUMMIT;	/* Cambiamos el proceso de RUNNING a SUMMIT */
	
	if(procesess[Task_id].state == SUMMIT){
		procesess[Task_id].state = RUNNING;
		procesess[Task_id].continuePointer = procesess[Task_id].initPointer;
		
	} else {
		procesess[Task_id].state = RUNNING;
	}
	
	pc_actual = procesess[Task_id].continuePointer;
	running_task = Task_id;
	
	asm{
		LDHX	pc_actual
		JMP		,X
	}
}

void Terminate_Task(void){
	if(running_task != 0){
		procesess[running_task].state = SUMMIT;
	}
	Schedule();
}

TASK(ROOT){
	asm{
		NOP
	}
	Terminate_Task();
}
