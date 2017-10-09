#include "Alarm.h"
#include "project.h"
#include "Task.h"

Alarm_t alarm[ALARM_LIMIT];
unsigned char loopi = 0;

CY_ISR(Ticker);

void init_alarms(unsigned long ticks){
    CyIntSetSysVector((SysTick_IRQn + 16), Ticker);
    (void)SysTick_Config(ticks);
}

void SetAlarm(unsigned char alarm_id, unsigned char task_id, unsigned int ticks, unsigned char repetitive){
    alarm[alarm_id].ID = alarm_id;
    alarm[alarm_id].PID = task_id;
    alarm[alarm_id].Ticks = ticks;
    alarm[alarm_id].SavedTicks = ticks;
    alarm[alarm_id].Repetitive = repetitive;
}

CY_ISR(Ticker){
    CyGlobalIntDisable;
    do{
		if(alarm[loopi].State == ALARM_STATE_ACTIVATED){
			alarm[loopi].Ticks--;
			if(alarm[loopi].Ticks == 0){
				Task[alarm[loopi].PID].State = TASK_STATE_READY;
				if(alarm[loopi].Repetitive){
					alarm[loopi].Ticks = alarm[loopi].SavedTicks;
				} else {
					alarm[loopi].State = ALARM_STATE_DESACTIVATED;
				}
			}
		}
	} while(++loopi <= ALARM_LIMIT);
    loopi = 0;
    CyIntClearPending(SysTick_IRQn + 16);
    CyGlobalIntEnable;
}