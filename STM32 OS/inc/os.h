#include "stm32f4xx.h"
#include "Task.h"

/* Definiciones de un proceso */
#define Task(X)	static void X(void)
#define TASK(X)	static void X(void)
#define task(X)	static void X(void)

/* Definiciones de una interrupción */
#define Isr(X)	 void X(void)
#define ISR(X)	 void X(void)
#define isr(X)	 void X(void)

unsigned char os_start(void);

