#define DeclareTask(x) void x()
	/* Prototipa la funcion de la tarea */
#define TASK(x) void x()
	/* Syntaxis para definir las tareas del usuario */

void createTask(unsigned char task_id, unsigned char priority ,unsigned char autostart, void *(task)());
	/* Crea un proceso. El usuario debe colocar el numero del proceso, la prioridad, , si sera
	 * iniciado por el sistema operativo y la funcion que ejecutara el task  */

void init_OS(void);
	/* Inicializa el sistema operativo. Busca en el arreglo los procesos todos los que tengan 
	 * habilitada la opcion de autostart y después coloca estos procesos en el estado de READY.
	 * Al final se llama a la funcion de Schedule() para que se comiezen a correr */

volatile void Activate_Task(unsigned char Task_id);
	/* Especifica cual tarea será transferida del estado SUMMIT al estado Ready */

void Terminate_Task(void);
	/* Manda la tarea del estado RUNNING al estado SUMMIT */

void Chain_Task(unsigned char Task_id);
	/* Termina el proceso donde se llamo a la funcion y llama a otro proceso indicado */

void Schedule(void);
	/* Cada vez que es llamada la función esta busca el proceso con mayor prioridad que este en el estado de Ready
	 * y la cambia al estado de Running. Esta se llama cada vez que se realiza un cambio en la lista de procesos. */

