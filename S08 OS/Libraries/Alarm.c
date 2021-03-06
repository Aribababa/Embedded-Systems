#include "Alarm.h"
#include "Task.h"
#include "OSv2.h"
#include <mc9s08qg8.h>
#include <hidef.h> 

#define ResetLoopi	loopi = 0

Alarm_t alarm[10];		/* Creamos las diez alarmas permitidas por el sistema */
unsigned char loopi = 0;

void init_alarms(unsigned char miliseconds){
	SOPT1 = 0x52;	/* Desactivamos al perro */
	
	MTIMSC = 0x40;
	MTIMCLK = 0x08; 
	MTIMMOD = miliseconds;
}

void SetAlarm(unsigned char alarm_id, unsigned char task_id, unsigned int ticks, unsigned char repetitive){
	alarm[alarm_id].Alarm_id = (alarm_id <= 10 && alarm_id > 0) ? alarm_id : 0x01;
	alarm[alarm_id].Task_id = task_id;
	alarm[alarm_id].Ticks = ticks;
	alarm[alarm_id].SavedTicks = ticks;
	alarm[alarm_id].Repetitive = repetitive;
	alarm[alarm_id].State = ACTIVATED;
}

void ActivateAlarm(unsigned char alarm_id){
	alarm[alarm_id].State = ACTIVATED;
	
}

void DesactivateAlarm(unsigned char alarm_id){
	alarm[alarm_id].State = DESACTIVATED;
}

interrupt 12 void tick(void){
	DisableInterrupts;
	do{
		if(alarm[loopi].State == ACTIVATED){
			alarm[loopi].Ticks--;
			if(alarm[loopi].Ticks == 0){
				ActivateTask_Alarm(alarm[loopi].Task_id);
				if(alarm[loopi].Repetitive){
					alarm[loopi].Ticks = alarm[loopi].SavedTicks;
				} else {
					alarm[loopi].State = DESACTIVATED;
				}
			}
		}
	} while(++loopi <= 10);
	ResetLoopi;
	(void)MTIMSC;
	MTIMSC_TOF = 0;
	EnableInterrupts;
}
