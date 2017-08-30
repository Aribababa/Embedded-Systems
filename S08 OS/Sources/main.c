#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include <OS.h>

DeclareTask(TASK_A);
DeclareTask(TASK_B);
DeclareTask(TASK_C);
DeclareTask(TASKERINO);


void main(void) {
	KBISC = 0x02;
	KBIPE = 0x08;
	KBIES = 0x00;
	
	CreateTask(1,0 , AUTOSTART, &TASK_A);
	CreateTask(2, 1, NO_AUTOSTART, &TASK_B);
	CreateTask(3, 2, NO_AUTOSTART, &TASK_C);
	CreateTask(4, 3, NO_AUTOSTART, &TASKERINO);
	
	SOPT1 = 0x52;
	EnableInterrupts;
	
	
	Init_OS();

  for(;;){
	  
  }

}

TASK(TASK_A){
	PTADD_PTADD0 = 1;
	
	__asm NOP;
	ActivateTask(2);
	__asm NOP;
	TerminateTask();
}

TASK(TASK_B){
	__asm NOP;
	ChainTask(3);
}

TASK(TASK_C){
	__asm NOP;
	TerminateTask();
}

TASK(TASKERINO){
	__asm NOP;
	TerminateTask();
}

ISR(Keyboard_ISR){
	KBISC_KBACK = 0x01;
	ActivateTask_ISR(3);
}
