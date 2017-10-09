#include "OS.h"
#include "project.h"
#include "Task.h"
#include "Alarm.h"

unsigned char looper = 0;

/* Proceso que corre el OS cuando no hay nada que realizar */
static void ROOT(void){
    /* Generamos delay hará que se pueda interrumpir el sistema */
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    TerminateTask();
}

void Start_OS(void){
    CyGlobalIntDisable; 
    CreateTask(0, ROOT,0, AUTOSTART);   /* Creamos la tarea que corre por default en el sistema */
    init_alarms(_1ms);
    do{
        if(Task[looper].Autostart == AUTOSTART){
            Task[looper].State = TASK_STATE_READY;
        }
    } while(++looper <= TASK_LIMIT);
    looper = 0;
    
    (void) Schedule();
    
    __asm("POP {r0-r1}");
    __asm("MOV  R7, SP");       /* Guardamos la direccion del Stack en R7*/
    __asm("MOV  R0, %0" : : "r" (Task[current_task].LinkRegister));
    __asm("STR  R0, [R7, #0x04]");  /* Colocamos un valor en LR */
    CyGlobalIntEnable;
}

void Schedule(void){
    do{
        if(Task[looper].State == TASK_STATE_READY || Task[looper].State == TASK_STATE_WAITING){
            if(Task[looper].Priority >= 0 && Task[looper].LinkRegister != 0 ){
                current_task = looper;    
            }
        }
    } while(++looper <= TASK_LIMIT);
    looper = 0;
}
