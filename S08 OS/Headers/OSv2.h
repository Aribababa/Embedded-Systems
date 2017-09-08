

/* Facilitamos la creación de Tareas para el usuario */
#define	DeclareTask(x)	void x(void)
#define TASK(X)		void X(void)

/*Facilitamos las interrupciones utilizando macros */
extern unsigned char loader;
extern unsigned int *next_task;

void init_OS(void);

extern void Schedule(void);

