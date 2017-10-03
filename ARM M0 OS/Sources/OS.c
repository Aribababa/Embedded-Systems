#include "OS.h"
#include "project.h"
#include "Task.h"

unsigned char looper = 0;

/* Proceso que corre el OS cuando no hay nada que realizar */
static void ROOT(void){
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
    do{
        if(Task[looper].Autostart == AUTOSTART){
            Task[looper].State = TASK_STATE_READY;
        }
    } while(++looper <= TASK_LIMIT);
    looper = 0;  
    (void)Schedule();
}

void Schedule(void){
    CyGlobalIntDisable;
    do{
        if(Task[looper].State == TASK_STATE_READY){
            if(Task[looper].Priority > Task[0].Priority){
                current_task = looper;    
            }
        }
    } while(++looper <= TASK_LIMIT);
    looper = 0;
    
    /* Preparamos el proceso para ser ejecutado */
    Task[current_task].State = TASK_STATE_RUNNING;
    __asm("MOV lr, %0" : : "r" (Task[current_task].LinkRegister));   /* Set LR */
    __set_MSP((unsigned int)Task[current_task].Stack);
    CyGlobalIntEnable;  /* Volvemos a habilitar las interrupciones */ 
    __asm("POP {r4 - r7}      \n");
    __asm("BX lr");
}

