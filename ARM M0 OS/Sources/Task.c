#include "Task.h"
#include "OS.h"
#include "project.h"

Task_t Task[TASK_LIMIT];
unsigned int user_stacks[TASK_LIMIT][STACK_SIZE];   
unsigned char current_task = 0; /* Se inicia en cero para que apunte al proceso por default */

void CreateTask(unsigned char task_id, void (*task_function)(void), unsigned char priority, unsigned char autostart){
    /* Colocamos la información general del proceso */
    Task[task_id].PID = (unsigned char)task_id;
    Task[task_id].Priority = priority;
    Task[task_id].State = TASK_STATE_SUSPENDED;
    Task[task_id].Autostart = autostart;
    Task[task_id].LinkRegister = (unsigned int)task_function;
    Task[task_id].initAddress = (unsigned int)task_function;
    Task[task_id].Stack = user_stacks[task_id];
}  

void ActivateTask(unsigned char task_id){
    CyGlobalIntDisable;

     /* Cambiamos el estado de las tareas */
    Task[task_id].State = TASK_STATE_READY;
    Task[current_task].State = TASK_STATE_READY;
    
    /* Guardamos donde se quedo la tarea */
    __asm("MOV %0, lr" : "=r" (Task[current_task].LinkRegister));
    
    (void) Schedule();
    
       /* Pasamos a ejecutar la siguiente tarea */
    __asm("POP {r0-r1}");
    __asm("MOV  R7, SP");       /* Guardamos la direccion del Stack en R7*/
    __asm("MOV  R0, %0" : : "r" (Task[current_task].LinkRegister));
    __asm("STR  R0, [R7, #0x0C]");  /* Colocamos un valor en LR */
    CyGlobalIntEnable;
}

void ActivateTask_ISR(unsigned char task_id){
     CyGlobalIntDisable;
    
    /* Cambiamos el estado de las tareas */
    Task[task_id].State = TASK_STATE_READY;
    Task[current_task].State = TASK_STATE_READY;
    
    /* Buscamos el siguiente proceso a correr */
    (void) Schedule();
    
    /* Comenzamos a guardar el Stack */
    __asm("MOV  R7, SP");       /* Guardamos la direccion del Stack en R7*/
    __asm("MOV  R0, %0" : : "r" (Task[current_task].LinkRegister));
    __asm("STR  R0, [R7, #0x30]");  /* Colocamos un valor en LR */
    CyGlobalIntEnable;
}

void ChainTask(unsigned char task_id){
    CyGlobalIntDisable;
    
    /* Cambiamos el estado de las tareas */
    Task[task_id].State = TASK_STATE_RUNNING;
    Task[current_task].State = TASK_STATE_SUSPENDED;
    
    /* Cambiamos la tarea que está corriendo el sistema */
    current_task = task_id;
    
    /* Pasamos a ejecutar la siguiente tarea */
    __asm("POP {r0-r1}");
    __asm("MOV  R7, SP");       /* Guardamos la direccion del Stack en R7*/
    __asm("MOV  R0, %0" : : "r" (Task[current_task].LinkRegister));
    __asm("STR  R0, [R7, #0x0C]");  /* Colocamos un valor en LR */
    CyGlobalIntEnable;
}

void TerminateTask(void){
    CyGlobalIntDisable;
    if(current_task){ 
        Task[current_task].State = TASK_STATE_SUSPENDED; 
    }
    
    /* Preparamos la tarea por si es necesario llamarla otra vez */
    Task[current_task].LinkRegister = Task[current_task].initAddress;
    
    /* Reiniciamos la tarea actual y buscamos la siguiente a ejecutar */
    current_task = 0;
    (void) Schedule();
    
    /* Pasamos a ejecutar la siguiente tarea */
    __asm("POP {r0-r1}");
    __asm("MOV  R7, SP");       /* Guardamos la direccion del Stack en R7*/
    __asm("MOV  R0, %0" : : "r" (Task[current_task].LinkRegister));
    __asm("STR  R0, [R7, #0x04]");  /* Colocamos un valor en LR */
    CyGlobalIntEnable;
}
