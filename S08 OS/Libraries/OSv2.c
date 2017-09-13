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
	init_alarms(_1ms);	/* Configuramos el perido de los Ticks del sistema */
	do {
		if (task[looper].State == SUSPENDED && task[looper].Autostart) {
			task[looper].State = READY;
		}
	} while (++looper <= MAX_TASKS);
	ResetLooper;
	__asm ais #-4;	/* Evita un ¡GRAN! problema a futuro */
	EnableInterrupts;
	(void)Schedule();
}

extern void Schedule(void){
	DisableInterrupts;	/* Hay que hacer esto por salud mental y estomacal */
	ResetLooper;
	do {	/* Buscamos el siguiente proceso a correr */
		if (task[looper].State == READY) {
			if (task[looper].Priority >= task[0].Priority) {
				running_task = looper;
			}
		}
	} while (++looper <= MAX_TASKS);
	next_task = task[running_task].TCB.ProgramCounter;
	ResetLooper;
	
	/* Hay que verifiar si el proceso al que se va a saltar fue interrumpido o no. */
	if(task[running_task].Interrupted){
		task[running_task].Interrupted = 0;
		RestoreContext_ISR();
	} else {
		RestoreContext();
	}
}





