#include "Task.h"
#include "OS.h"
#include "IQMath.h"

/* Para que el usuario defina las tareas de manera más sencilla */
#define task(X)    static void X(void)
#define Task(X)    static void X(void)
#define TASK(X)    static void X(void)

/* Para las interrupciones */
#define isr(X)  CY_ISR(X);
#define Isr(X)  CY_ISR(X);
#define ISR(X)  CY_ISR(X);

