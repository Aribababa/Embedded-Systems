#include <alarm.h>
#include <mc9s08qg8.h>
#include <hidef.h> 


Alarm_t alarm[10];		/* Creamos las diez alarmas permitidas por el sistema */
unsigned char alarm_index;		/* Indica hasta donde tendrá que recorrer el arreglo */

void init_alarms(void){
	MTIMCLK = 0x02;	/* Preescalamos el reloj a 1 MHz */
	MTIMCNT = 100;	/* Contará hasta 100 antes de causar una interrupción */
	MTIMSC = 0x40;	/* Activamos las interrupciones de este módulo */
}

void ActivateAlarm(unsigned char alarm_id, unsigned char task_id, unsigned int ticks, unsigned char repetitive){
	alarm_index++;
}

void DesactivateAlarm(unsigned char alarm_id){
	unsigned char i = 0;
	do{
		if(alarm[i].Alarm_id == alarm_id){
			alarm[i].State = DESACTIVATED;
		}
	} while(++i <= alarm_index);
}

interrupt 12 void CHECK_ALARMS(void){
	MTIMSC_TOF = 0;	/* Apagamos la bandera de la interrupción */
}

