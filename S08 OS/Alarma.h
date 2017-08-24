#define DeclareAlarm(x) void x()
	/* Prototipa la funcion de alarma */
#define ALARM(x) void x()
	/* Definir alarmas */

void Activa_Alarma(unsigned char alarmNo, unsigned char task_id, unsigned int ticks, unsigned char repetitivo);
	/* Activa una de las diez alarmas */

void Desactiva_Alarma(unsigned char alarmNo);
	/* Manda la alarma al estado APAGADO */

void clear_MTIMSC_TOF(void);

void inicializaMTIM(void);
