#include "os.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


static void swap(task_t *xp, task_t *yp);
static void OrderByPriority(task_t arr[], uint8_t n);

/* Inicia el sistema operativo, busca la primera tarea i comineza con las
  interrupciones para el Scheduler y el cambio de contexto */
unsigned char os_start(void){

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);

	if(!m_task_table.size){
		return 0;
	}

	/* Configuramos las interrrupciones del sistema */
	NVIC_SetPriority(PendSV_IRQn, 0xFF); 	/* Menor prioridad posible */
	NVIC_SetPriority(SysTick_IRQn, 0x00); 	/* Mayor prioridad posible */

	/* Iniciamos el SysTick del sistema  */
	uint8_t ret_val = SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);
	if (ret_val != 0){
		return 0;
	}

	/* Acomodamos los procesos por orden de prioridad,  por lo que habrá que hacer un
	  algoritmo de acomodo */
	OrderByPriority(m_task_table.tasks ,m_task_table.size);

	/* Buscamso el primer proceso en ejecutarse. Si no hay ninguno dispoible se quedará en el loop
	 * hasta que uno entre en el estado de READY */
	while(1){
		if(m_task_table.tasks[m_task_table.current_task].status == OS_TASK_STATUS_READY){
			break;
		}
		m_task_table.current_task = (m_task_table.current_task+1)%m_task_table.size;
	}
	os_curr_task = &m_task_table.tasks[m_task_table.current_task];

	__set_PSP(os_curr_task->sp+STACK_SIZE); /* Set PSP to the top of task's stack */
	__set_CONTROL(0x03); 			/* Cambiamos al modo de Process Stack Pointer( PSP) no privilegiado  */
	__ISB(); 	/* Despues de cambiar el registro de control, hay que ejecutar esto para que funcione correctamente */

	os_curr_task->handler();

	return 1;
}

/* Scheduler del sistema */
void SysTick_Handler(void){

	os_curr_task = &m_task_table.tasks[m_task_table.current_task];
	os_curr_task->status = OS_TASK_STATUS_READY;

	while(1){
		m_task_table.current_task = (m_task_table.current_task+1)%m_task_table.size;
		if(m_task_table.tasks[m_task_table.current_task].status == OS_TASK_STATUS_READY){
			break;
		}
	}
	__disable_irq();	/* Hay que hacer esto por salud mental y estomacal */


	os_next_task = &m_task_table.tasks[m_task_table.current_task];
	os_next_task->status = OS_TASK_STATUS_RUNNING;

	/* PendSV es quien hace el cambio de contexto : */
	__enable_irq();	/* Hay que hacer esto por salud mental y estomacal */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/* Acomoda los procesos por orden de prioridad. Para evitar problemas con la búsqueda
 * de procesos durante la ejecucion */
static void OrderByPriority(task_t arr[], uint8_t n){
	uint8_t i, j, swapped;
	for (i = 0; i < n-1; i++){
		for (j = 0; j < n-i-1; j++){
			if (arr[j].priority > arr[j+1].priority){
				swap(&arr[j], &arr[j+1]);
				swapped = 1;
			}
		}
		if (!swapped){	/* Si no hubo algun cambio, sabemos que ya estan en orden y rompemos el loop */
			break;
		}
	}
}

/* Realiza el cambio de posicion en el arreglo de tareas */
static void swap(task_t *xp, task_t *yp){
	task_t temp = *xp;
	*xp = *yp;
	*yp = temp;
}

