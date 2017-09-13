#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "OSEKduino.h"

unsigned char PWM_01_flag;
unsigned char PWM_02_flag;
unsigned char PWM_03_flag;
unsigned char PWM_04_flag;


unsigned char duty_cycle_pwm01 = 75;
unsigned char duty_cycle_pwm02 = 25;
unsigned char duty_cycle_pwm03 = 30;
unsigned char duty_cycle_pwm04 = 60;

/* Para los PWM */
DeclareTask(PWM_01);	/* PWM con Duty Cicle de 20% */
DeclareTask(PWM_02);
DeclareTask(PWM_03);
DeclareTask(PWM_04);
DeclareTask(PWM_05);
DeclareTask(PWM_06);
DeclareTask(TASK_A);

void main(void) {
	PTBDD = 0xFF;	/* Declaramos */
	
	SRTISC = 0x12;
	SOPT1 = 0x52;
	
	CreateTask(1, 10, NO_AUTOSTART, &PWM_01);
	CreateTask(2, 12, NO_AUTOSTART, &PWM_02);
	CreateTask(3, 14, NO_AUTOSTART, &PWM_03);
	CreateTask(4, 16, NO_AUTOSTART, &PWM_04);
	
	CreateTask(5, 18, NO_AUTOSTART, &PWM_05);
	CreateTask(6, 20, NO_AUTOSTART, &PWM_06);
	
	CreateTask(7, 22, NO_AUTOSTART, &TASK_A);
	
	SetAlarm(1, 1, 1, REPETITIVE);
	SetAlarm(2, 2, 1, REPETITIVE);
	SetAlarm(3, 3, 1, REPETITIVE);
	SetAlarm(4, 4, 1, REPETITIVE);
	
	EnableInterrupts;
	init_OS();
	
	for(;;);
}

Task(PWM_01){
	if(PWM_01_flag%2){
		PTBD_PTBD7 = 0;
		SetAlarm(1, 1, 100 - (duty_cycle_pwm01), REPETITIVE);
	} else {
		PTBD_PTBD7 = 1;
		SetAlarm(1, 1, (duty_cycle_pwm01), REPETITIVE);
	}
	PWM_01_flag++;
	TerminateTask();
}

Task(PWM_02){
	if(PWM_02_flag%2){
		PTBD_PTBD6 = 0;
		SetAlarm(2, 2, 100 - (duty_cycle_pwm02), REPETITIVE);
	} else {
		PTBD_PTBD6 = 1;
		SetAlarm(2, 2,(duty_cycle_pwm02), REPETITIVE);
	}
	PWM_02_flag++;
	TerminateTask();
}

Task(PWM_03){
	if(PWM_03_flag%2){
		PTBD_PTBD5 = 0;
		SetAlarm(3, 3, 100 - (duty_cycle_pwm03), REPETITIVE);
	} else {
		PTBD_PTBD5 = 1;
		SetAlarm(3, 3, duty_cycle_pwm03, REPETITIVE);
	}
	PWM_03_flag++;
	TerminateTask();
}

Task(PWM_04){
	if(PWM_04_flag%2){
		PTBD_PTBD4 = 0;
		SetAlarm(4, 4, 100 - (duty_cycle_pwm04), REPETITIVE);
	} else {
		PTBD_PTBD4 = 1;
		SetAlarm(4, 4, duty_cycle_pwm04, REPETITIVE);
	}
	PWM_04_flag++;
	TerminateTask();
}

Task(PWM_05){
	TerminateTask();
}

Task(PWM_06){
	__asm NOP;
//	do{
//		/* Nos quedamos aqui hasta que el dato este disponible para usarlo */
//		__NOP();
//	} while(WaitMessage(1));
	TerminateTask();
}

Task(TASK_A){
	/* Esta tarea ayuda para probar las interrupciones */
	u8 Arduino_Uno = 0x30;
	u8 Arduino_Yun = 0x31;
	u8 Arduino_Mini = 0x32;
	u8 Arduino_Nano = 0x33;
	u8 Arduino_Micro = 0x34;
	
	//PTBD_PTBD4 = ~PTBD_PTBD4;
	TerminateTask();
}

ISR(RTC_ISR){
	/* Interrupcion periodica para probar el sistema de interrupciones
	 * del sistema operativo */
	SRTISC_RTIACK = 1;
	ActivateTask_ISR(7);	/* Activa esta tarea y se visualiza si se activó o no la tarea */
}
