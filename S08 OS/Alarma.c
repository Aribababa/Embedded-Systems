#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/** 2 POSIBLES ESTADOS DE LA ALARMA ------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
#define APAGADA 0
#define PRENDIDA 1
/** 10 ALARMAS CÓMO MÁXIMO ---------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
#define MAX_ALARMS 10

/** VARIABLES ----------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
unsigned char alarm_exists;

/** ESTRUCTURA DE LA ALARMA --------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
struct AlarmStructure{
	unsigned char task_id;		// ID del proceso a tratar
	unsigned int  ticks;		// Unidad de tiempo (max. 100 ticks)
	unsigned char ticksBackup;	// Reserva de ticks en caso de que sea repetitivo
	unsigned char repetitivo;	// Determina si es repetitivo o no
	unsigned char alarmState;	// Determina si el estado es prendido/apagado
	
}; 

struct AlarmStructure alarmas[MAX_ALARMS];	// Estructura con 10 Alarmas

void Crea_Alarma(unsigned char alarmNo, unsigned char task_id, unsigned int ticks, unsigned char repetitivo)
{
	if(alarm_exists == 0)
	{
		alarmas[0].alarmState = APAGADA;
		alarmas[0].task_id = task_id;
		alarmas[0].ticks = ticks;
		alarmas[0].repetitivo = 0;
	}
}


/** ACTIVA ALARMA ------------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
void Activa_Alarma(unsigned char alarmNo, unsigned char task_id, unsigned int ticks, unsigned char repetitivo)
{
	alarmas[alarmNo].alarmState = APAGADA;		// Alarmas apagadas por default
	alarmas[alarmNo].task_id = task_id;			// ID del proceso
	alarmas[alarmNo].ticks = ticks;				// Número de ticks
	alarmas[alarmNo].repetitivo = 0;			// No repetitivo por default
}

/** DESACTIVA ALARMA ---------------------------------------------------------------------------------------------- **/
/** --------------------------------------------------------------------------------------------------------------- **/
void Desactiva_Alarma(unsigned char alarmNo)
{
	alarmas[alarmNo].alarmState = APAGADA;
}

/* Módulo MTIMSC */
void inicializaMTIM(void)
{
		MTIMMOD = 255; /* El módulo contara hasta MITMMOD y despues se interrumpirá */
		MTIMSC = 0x40; /* Interrupciones habilitadas */
		MTIMCLK = 0x08; /* BUSCLK y  prescaler a 256 (31.25KHz)*/
}

void clear_MTIMSC_TOF(void) // Limpia TOF del  MTIM
{	
	(void) MTIMSC;
	MTIMSC_TOF = 0;
}

interrupt 12 void MTIMSC_INTERRUPT(void) 
{
	clear_MTIMSC_TOF();
	Activa_Alarma(9,10,1000,0);
}





