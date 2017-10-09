#define ALARM_LIMIT 10

#define REPETITIVE      ('R')
#define NON_REPETITIVE  ('N')

/* Velocidad de las alarmas */
#define _1ms        48000
#define _100us      4800
#define _10us       480
#define _1us        48

typedef enum{
    ALARM_STATE_ACTIVATED,
    ALARM_STATE_DESACTIVATED
} AlarmState;

typedef struct{
    unsigned char ID;
    unsigned char PID;
    unsigned int Ticks;
    unsigned int SavedTicks;
    unsigned char Repetitive;
    AlarmState State;
} Alarm_t;

void init_alarms(unsigned long ticks);
	/* Inicializa el sistema de alarmas */

void SetAlarm(unsigned char alarm_id, unsigned char task_id, unsigned int ticks, unsigned char repetitive);

void ActivateAlarm(unsigned char alarm_id);
	/* Crea una alarma que se activara dependiento del número de Ticks recividos por el parámetro. Tambien se le da 
	 * cual tarea activará y si la alarma será repetitiva o no. */

void DesactivateAlarm(unsigned char alarm_id);