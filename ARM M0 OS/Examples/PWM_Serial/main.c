#include "project.h"
#include "OSEKduino.h"


unsigned char PWM01_Flag;
unsigned char PWM02_Flag;
unsigned char PWM03_Flag;
unsigned char PWM04_Flag;

unsigned char PWM01_DutyCycle;
unsigned char PWM02_DutyCycle;
unsigned char PWM03_DutyCycle;
unsigned char PWM04_DutyCycle;

/* Declaramos las tareas de Sistema */
Task(Wait_Start_Bit);
Task(Sample_Rx_01);

Task(PWM_Signal_01);

/* Prototipamos las interrupciones */
ISR(Timer_01_Handler);
ISR(Timer_02_Handler);

int main(void){
    /* Creamos las tareas */
    CreateTask(1, &Wait_Start_Bit, 1, NO_AUTOSTART);
    CreateTask(2, &Sample_Rx_01, 100, NO_AUTOSTART);
    //CreateTask(3, &Sample_Rx_02, 100, NO_AUTOSTART);
    //CreateTask(4, &Sample_Rx_03, 100, NO_AUTOSTART);
    //CreateTask(5, &Sample_Rx_04, 100, NO_AUTOSTART);
    CreateTask(6, &PWM_Signal_01, 50, NO_AUTOSTART);
    
    
    /* Creamos la alarmas del sistema */
    SetAlarm(1,1,1,REPETITIVE);
    
    /* Inicializamos las interrupciones */
    Timer_01_ISR_StartEx(Timer_01_Handler);
    Timer_02_ISR_StartEx(Timer_02_Handler);
    
    /* Iniciamos los periféricos */
    Timer_01_Stop();
    Timer_02_Stop();
    
    CyGlobalIntEnable;
    Start_OS();
    
    for(;;);   /* Si llegaste a este punto, algo hiciste mal */
}

/* Leemos los pines para detectar un start bit en la comunicación  */
Task(Wait_Start_Bit){
    if(!Rx_01_Read()){
        Timer_01_Start();
    }
    TerminateTask();
}

Task(Sample_Rx_01){
    PWM01_DutyCycle <<= 1;
    if(Rx_01_Read()){
        PWM01_DutyCycle++;
    }
    PWM01_Flag++;
    if(PWM01_Flag == 7){
        Timer_01_Stop();    
        ActivateTask(6);
    }
    TerminateTask();
}

Task(PWM_Signal_01){
    if(PWM01_Flag%2){
        PWM_1_Write(1);
        SetAlarm(2,6,PWM01_DutyCycle,REPETITIVE);
    } else {
        PWM_1_Write(0);
        SetAlarm(2,6,100-PWM01_DutyCycle,REPETITIVE);
    }
    PWM01_Flag++;
    TerminateTask();
}

/* Interrupciones del sistsema */
ISR(Timer_01_Handler){
    Timer_01_ISR_ClearPending();
    ActivateTask_ISR(2);
}

ISR(Timer_02_Handler){
    Timer_02_ISR_ClearPending();
    ActivateTask_ISR(3);
}




