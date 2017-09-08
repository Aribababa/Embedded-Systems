#include "OSV2.h"
#include "Task.h"
#include "Alarm.h"
#include "Mailbox.h"

/* Definimos varias maros para el usuario */
#define u8 unsigned char
#define u16 unsigned int

/* Definimos macros para ls interrpuciones */

#define ISR(X)	interrupt X
#define IRQ(X)	interrupt X
#define INTERRUPT(X)	interrupt X

#define ADC_ISR			19 void ADC_INTERRUPT(void)
#define Keyboard_ISR	18 void KEYBOARD_INTERRUPT(void)
#define I2C_ISR			17 void I2C_INTERRUPT(void)
#define UART_TX_ISR		16 void SCI_TX_INTERRUPT(void)
#define UART_RX_ISR		15 void SCI_RX_INTERRUPT(void)
#define UART_ERROR_ISR	14 void SCI_ERROR_INTERRUPT(void)
#define SPI_ISR			13 void SPI_INTERRUPT(void)
#define TIMER_ISR		12 void MTIM_INTERRUPT(void)
#define PWM_ISR			7 void TPM_INTERRUPT(void)
#define PWM_CH1_ISR		6 void TPM_CH1_INTERRUPT(void)
#define PWM_CH0_ISR		5 void TPM_CH0_INTERRUPT(void)
