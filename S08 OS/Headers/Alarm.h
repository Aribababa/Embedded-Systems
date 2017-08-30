#define DESACTIVATED	0
#define ACTIVATED		1

#define NO_REPETITVE 0
#define REPETITIVE	 1

typedef struct Alarm{
	unsigned char Alarm_id;
	unsigned char Task_id;
	unsigned int Ticks;
	unsigned int SavedTicks;
	unsigned char State;
	unsigned char Repetitive; 
	
} Alarm_t;

void init_alarms(void);
	/* Inicializa el sistema de alarmas */

void ActivateAlarm(unsigned char alarm_id, unsigned char task_id, unsigned int ticks, unsigned char repetitive);
	/* Crea una alarma que se activara dependiento del número de Ticks recividos por el parámetro. Tambien se le da 
	 * cual tarea activará y si la alarma será repetitiva o no. */

void DesactivateAlarm(unsigned char alarm_id);
	/* Desactiva la alarma que se recive por el parámetro. */


