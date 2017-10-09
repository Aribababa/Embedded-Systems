#include "project.h"
#include "OSEKduino.h"
#include "IQMath.h"

unsigned char Arduino = 0x25;

/* Declaramos las tareas de Sistema */
Task(Task_A);
Task(Task_B);
Task(Task_C);
Task(Task_D);

/* Prototipamos las interrupciones */
ISR(Button_Handler);

int main(void){
    
    /* Creamos las tareas del sistema */
    CreateTask(1, &Task_A, 1, NO_AUTOSTART);
    CreateTask(2, &Task_B, 1, NO_AUTOSTART);
    CreateTask(3, &Task_C, 1, NO_AUTOSTART);
    CreateTask(4, &Task_D, 244, NO_AUTOSTART);
    
    /* Creamos alarmas */
    SetAlarm(2,2,70,REPETITIVE);
    SetAlarm(3,3,10,REPETITIVE);
    
    /* Iniciamos la interrupcion de prueba */
    //Button_ISR_StartEx(Button_Handler);
    
    /* Activamos las interrupciones e iniciamos el sistema operativo */
    CyGlobalIntEnable;
    Start_OS();
    
    for(;;);   /* Si llegaste a este punto, algo hiciste mal */
}

Task(Task_A){
    TerminateTask();
}

Task(Task_B){  
    G_LED_Write(!G_LED_Read());
    TerminateTask();
}

Task(Task_C){
    B_LED_Write(!B_LED_Read());
    TerminateTask();
}

Task(Task_D){
    R_LED_Write(!R_LED_Read());
    TerminateTask();
}

ISR(Button_Handler){
    Button_ClearInterrupt();
    Button_ISR_ClearPending();
    ActivateTask_ISR(4);
}


