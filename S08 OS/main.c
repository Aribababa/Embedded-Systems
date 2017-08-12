#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "OS.h"

#define NO_AUTOSTART 0
#define AUTOSTART 1

DeclareTask(TASK_A);
DeclareTask(TASK_B);
DeclareTask(TASK_C);

void main(void) {
	
	createTask(1, 0, AUTOSTART, TASK_A);
	createTask(2, 1, NO_AUTOSTART, TASK_B);
	createTask(3, 2, NO_AUTOSTART, TASK_C);
	
	SOPT1 = 0x52;	/* Do not feed the dog */
	EnableInterrupts;
	
	init_OS();

	for(;;) {}
}


TASK(TASK_A){
	PTADD = 0xFF;
	asm{
		NOP
	}
	Activate_Task(2);
	asm{
			NOP
		}
	Terminate_Task();
}

TASK(TASK_B){
	asm{
		NOP
	}
	Chain_Task(3);
}

TASK(TASK_C){
	asm{
		NOP
	}
	Terminate_Task();
}

