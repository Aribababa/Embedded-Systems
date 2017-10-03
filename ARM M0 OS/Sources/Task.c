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
    Task[task_id].Stack = user_stacks[task_id];
}  

void ActivateTask(unsigned char task_id){
    CyGlobalIntDisable;
    /* Colocamos en READY las tareas */
    __asm("MOV %0, lr" : "=r" (Task[current_task].LinkRegister) ); /* Get LR */
    __asm("PUSH {r4 - r7}       \n");
    Task[current_task].Stack = (unsigned int*)__get_MSP();
    Task[task_id].State = TASK_STATE_READY;
    Task[current_task].State = TASK_STATE_READY;
    /* Mantenemos una copia de MSP en el registro R11 */
    (void)Schedule();
    return;
}

void ChainTask(unsigned char task_id){
    CyGlobalIntDisable;
    __asm("MOV %0, lr" : "=r" (Task[current_task].LinkRegister) ); /* Get LR */
    __asm("PUSH {r4 - r7}   \n");
    Task[current_task].Stack = (unsigned int*)__get_MSP();
    Task[current_task].State = TASK_STATE_SUSPENDED;
    
    current_task = task_id;
    
    Task[task_id].State = TASK_STATE_RUNNING;
    __asm("MOV lr, %0" : : "r" (Task[task_id].LinkRegister));   /* Set LR */
    __set_MSP((unsigned int)Task[task_id].Stack);
    CyGlobalIntEnable;
    __asm("POP {r4 - r7}      \n");
    __asm("BX lr");
}

void TerminateTask(void){
    CyGlobalIntDisable;
    if(current_task){ 
        Task[current_task].State = TASK_STATE_SUSPENDED; 
    } 
    (void)Schedule();
    return;
}

unsigned int __get_LR(void){
    register unsigned int result; 
  __asm("MOV %0, LR\n" : "=r" (result) ); 
  return(result); 
}

void __set_LR(unsigned int LinkRegister){
    (void)LinkRegister;
    __asm("MOV  LR, r0");
}