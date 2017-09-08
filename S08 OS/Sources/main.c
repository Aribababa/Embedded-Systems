#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "OSEKduino.h"

unsigned char PWM_01_flag;
unsigned char PWM_02_flag;
unsigned char PWM_03_flag;
unsigned char PWM_04_flag;

unsigned char duty_cycle_pwm01 = 90;
unsigned char duty_cycle_pwm02 = 5;
unsigned char duty_cycle_pwm03 = 40;
unsigned char duty_cycle_pwm04 = 66;

/* Para los PWM */
DeclareTask(PWM_01);	/* PWM con Duty Cicle de 20% */
DeclareTask(PWM_02);
DeclareTask(PWM_03);
DeclareTask(PWM_04);

void main(void) {
	PTBDD = 0xF0;	/* Declaramos */
	SOPT1 = 0x52;
	
	CreateTask(1,10, NO_AUTOSTART, &PWM_01);
	CreateTask(2,11, NO_AUTOSTART, &PWM_02);
	CreateTask(3,12, NO_AUTOSTART, &PWM_03);
	CreateTask(4,13, NO_AUTOSTART, &PWM_04);
	
	SetAlarm(1, 1, 1, REPETITIVE);
	SetAlarm(2, 2, 1, REPETITIVE);
	SetAlarm(3, 3, 1, REPETITIVE);
	SetAlarm(4, 4, 1, REPETITIVE);
	
	EnableInterrupts;
	init_OS();
	
	for(;;);
}

/*
 * 
 * Formula de %D = 100*(T(on)/Periodo)
 * */

TASK(PWM_01){
	PTBD_PTBD7 = ~PTBD_PTBD7;
	if(PWM_01_flag%2){
		SetAlarm(1, 1, 100 - (duty_cycle_pwm01), REPETITIVE);
	} else {
		SetAlarm(1, 1, (duty_cycle_pwm01), REPETITIVE);
	}
	PWM_01_flag++;
	TerminateTask();
	//SetAlarm(1, 1, 8, REPETITIVE);
}

TASK(PWM_02){
	PTBD_PTBD6 = ~PTBD_PTBD6;
	if(PWM_02_flag%2){
		SetAlarm(2, 2, 100 - (duty_cycle_pwm02), REPETITIVE);
	} else {
		SetAlarm(2, 2,(duty_cycle_pwm02), REPETITIVE);
	}
	PWM_02_flag++;
	TerminateTask();
}

TASK(PWM_03){
	PTBD_PTBD5 = ~PTBD_PTBD5;
	if(PWM_03_flag%2){
		SetAlarm(3, 3, 100 - (duty_cycle_pwm03), REPETITIVE);
	} else {
		SetAlarm(3, 3, duty_cycle_pwm03, REPETITIVE);
	}
	PWM_03_flag++;
	TerminateTask();
}

TASK(PWM_04){
	
	PTBD_PTBD4 = ~PTBD_PTBD4;
	if(PWM_04_flag%2){
		SetAlarm(4, 4, 100 - (duty_cycle_pwm04), REPETITIVE);
	} else {
		SetAlarm(4, 4, duty_cycle_pwm04, REPETITIVE);
	}
	PWM_04_flag++;
	TerminateTask();
}
