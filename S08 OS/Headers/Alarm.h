#define DESACTIVATED	0
#define ACTIVATED		1

#define NO_REPETITVE 0
#define REPETITIVE	 1

/* Escalas para loas Ticks del sistema */
#define	_64us	1
#define	_128us	2
#define	_256us	4
#define _512us	8
#define _1ms	16
#define	_2ms	32
#define _4ms 	64
#define _8ms 	128

typedef struct Alarm{
	unsigned char Alarm_id;
	unsigned char Task_id;
	unsigned int Ticks;
	unsigned int SavedTicks;
	unsigned char State;
	unsigned char Repetitive; 
	
} Alarm_t;

void init_alarms(unsigned char miliseconds);
	/* Inicializa el sistema de alarmas */

void SetAlarm(unsigned char alarm_id, unsigned char task_id, unsigned int ticks, unsigned char repetitive);

void ActivateAlarm(unsigned char alarm_id);
	/* Crea una alarma que se activara dependiento del número de Ticks recividos por el parámetro. Tambien se le da 
	 * cual tarea activará y si la alarma será repetitiva o no. */

void DesactivateAlarm(unsigned char alarm_id);
	/* Desactiva la alarma que se recive por el parámetro. */


