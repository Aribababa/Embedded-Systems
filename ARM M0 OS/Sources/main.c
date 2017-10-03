#include "project.h"
#include "OSEKduino.h"

Task(Task_A);
Task(Task_B);
Task(Task_C);

int main(void){
    
    CreateTask(1, &Task_A, 1, AUTOSTART);
    CreateTask(2, &Task_B, 2, NO_AUTOSTART);
    CreateTask(3, &Task_C, 3, NO_AUTOSTART);
    
    CyGlobalIntEnable;

    Start_OS();
    for(;;);   /* Si llegaste a este punto, algo hiciste mal */
}

Task(Task_A){
    CY_NOP;
    ActivateTask(2);
    CY_NOP;
    TerminateTask();
}

Task(Task_B){
    CY_NOP;
    ChainTask(3);
}

Task(Task_C){
    CY_NOP;
    LED_Write(!LED_Read());
    TerminateTask();
}
