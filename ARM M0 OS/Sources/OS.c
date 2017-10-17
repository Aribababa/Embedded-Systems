#include "OS.h"
#include "project.h"
#include "Task.h"
#include "Alarm.h"

unsigned int SystemStack;
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
    
    
    /* Guardamos el valor minimo que pede tener el Stack del sistema 
        Servirá para hhacer reajuste en caso de un Stack Underflow. */ 
    SystemStack =__get_MSP() + 0x20;
    
    for(uint8_t i = 0; i < TASK_LIMIT; i++){
        if(Task[i].Autostart == AUTOSTART){
            Task[i].State = TASK_STATE_READY;
        }
    }
    
    (void) Schedule();
    
    __asm("MOV  R7, SP");       /* Guardamos la direccion del Stack en R7*/
    __asm("MOV  LR, %0" : : "r" (Task[current_task].LinkRegister));
    __set_MSP(SystemStack);
    CyGlobalIntEnable;
    __asm("MOV PC, LR");
}

void Schedule(void){
    for(uint8_t i = 0; i < TASK_LIMIT; i++){     
        if(Task[i].State == TASK_STATE_READY || Task[i].State == TASK_STATE_WAITING){
            if(Task[i].Priority >= 0 && Task[i].LinkRegister != 0 ){
                current_task = i;    
            }
        }
    }
}

/* Scheduler para las interrupciones */
void Reschedule(void){
    for(uint8_t i = 0; i < TASK_LIMIT; i++){     
        if(Task[i].State == TASK_STATE_READY || Task[i].State == TASK_STATE_WAITING){
            if(Task[i].Priority >= 0 && Task[i].LinkRegister != 0 ){
                current_task = i;    
            }
        }
    }
    
    /* Hacemos el reajuste del Stack */    
    __set_MSP(SystemStack + 0x08);
    __asm("MOV  LR, %0" : : "r" (Task[current_task].LinkRegister));
    CyGlobalIntEnable;
    __asm("MOV  PC, LR");
}