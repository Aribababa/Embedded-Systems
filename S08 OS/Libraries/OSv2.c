#include "OSv2.h"
#include "Task.h"
#include "Alarm.h"
#include <hidef.h> 

#define ResetLooper	looper = 0

unsigned char looper = 0;	/* Un iterador para las disttitas funciones del sistema operativo */
extern unsigned int *next_task;	/* Variable que apunta a la dirección de la siguiente tarea a ejecutar */
unsigned char loader;

void init_OS(void){
	DisableInterrupts;
	init_alarms();
	do {
		if (task[looper].State == SUSPENDED && task[looper].Autostart) {
			task[looper].State = READY;
		}
	} while (++looper <= MAX_TASKS);
	ResetLooper;
	EnableInterrupts;
	Schedule();
}

extern void Schedule(void){
	DisableInterrupts;
	do {	/* Buscamos el siguiente proceso a correr */
		if (task[looper].State == READY) {
			if (task[looper].Priority >= task[0].Priority) {
				running_task = looper;
			}
		}
	} while (++looper <= MAX_TASKS);
	next_task = task[running_task].TCB.ProgramCounter;
	ResetLooper;
	
	RestoreContext();
}
