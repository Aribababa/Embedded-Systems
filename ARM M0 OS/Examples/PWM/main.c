#include "project.h"
#include "OSEKduino.h"

unsigned char PWM1_flag;
unsigned char PWM2_flag;
unsigned char PWM3_flag;
unsigned char PWM4_flag;

unsigned char PWM_DC_1 = 33;
unsigned char PWM_DC_2 = 66;
unsigned char PWM_DC_3 = 99;
unsigned char PWM_DC_4 = 50;

/* Declaramos las tareas de Sistema */
Task(Task_A);
Task(Task_B);
Task(Task_C);
Task(Task_D);

/* Prototipamos las interrupciones */


int main(void){
    
    CreateTask(1, &Task_A, 2, NO_AUTOSTART);
    CreateTask(2, &Task_B, 2, NO_AUTOSTART);
    CreateTask(3, &Task_C, 2, NO_AUTOSTART);
    CreateTask(4, &Task_D, 2, NO_AUTOSTART);
    
    SetAlarm(1,1,1,REPETITIVE);
    SetAlarm(2,2,1,REPETITIVE);
    SetAlarm(3,3,1,REPETITIVE);
    SetAlarm(4,4,1,REPETITIVE);
    
    CyGlobalIntEnable;
    Start_OS();
    
    for(;;);   /* Si llegaste a este punto, algo hiciste mal */
}

Task(Task_A){
    if(PWM1_flag%2){
        PWM_1_Write(1);
        SetAlarm(1,1,PWM_DC_1,REPETITIVE);
    } else {
        PWM_1_Write(0);
        SetAlarm(1,1,100 - PWM_DC_1,REPETITIVE);
    }
    PWM1_flag++;
    TerminateTask();
}

Task(Task_B){
    if(PWM2_flag%2){
        PWM_2_Write(1);
        SetAlarm(2,2,PWM_DC_2,REPETITIVE);
    } else {
        PWM_2_Write(0);
        SetAlarm(2,2,100 - PWM_DC_2,REPETITIVE);
    }
    PWM2_flag++;
    TerminateTask();
}

Task(Task_C){
    if(PWM3_flag%2){
        PWM_3_Write(1);
        SetAlarm(3,3,PWM_DC_3,REPETITIVE);
    } else {
        PWM_3_Write(0);
        SetAlarm(3,3,100 - PWM_DC_3,REPETITIVE);
    }
    PWM3_flag++;
    TerminateTask();
}

Task(Task_D){
    if(PWM4_flag%2){
        PWM_4_Write(1);
        SetAlarm(4,4,PWM_DC_4,REPETITIVE);
    } else {
        PWM_4_Write(0);
        SetAlarm(4,4,100 - PWM_DC_4,REPETITIVE);
    }
    PWM4_flag++;
    TerminateTask();
}
