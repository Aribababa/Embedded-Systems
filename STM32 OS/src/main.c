#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "os.h"

static void delay(volatile uint32_t delay_ms);
Task(TASK_A);
Task(TASK_B);
Task(TASK_C);
Task(TASK_D);

int main(void){
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	/* Creamos las tareas del sistema (0 es la mayor prioridad)*/
	CreateTask(1, 0 , TASK_AUTOSTART, &TASK_A);
	CreateTask(2, 32 , TASK_AUTOSTART, &TASK_B);
	CreateTask(3, 0 , TASK_AUTOSTART, &TASK_C);
	CreateTask(4, 3 , TASK_AUTOSTART, &TASK_D);

	/* Comienza a ejecutar ele sistema operativo */
	while(!os_start());

	for(;;);
}

static void delay(volatile uint32_t time){
	while (time > 0)
		time--;
}

Task(TASK_A){ /* Green */
	while(1){
		STM_EVAL_LEDToggle(LED4);
		delay(1280000);
	}
	TerminateTask();
}

Task(TASK_B){	/* Red */
	while (1) {
		STM_EVAL_LEDToggle(LED5);
		delay(640000);
	}
	TerminateTask();
}

Task(TASK_C){	/* Blue */
	char foo = 3;
	while (1) {
		if(foo == 3){
			STM_EVAL_LEDToggle(LED6);
		}
		delay(320000);
	}
	TerminateTask();
}

Task(TASK_D){	/* Orange */
	char foo = 4;
	while (1) {
		if(foo == 4){
			STM_EVAL_LEDToggle(LED3);
		}
		delay(160000);
	}
	TerminateTask();
}

