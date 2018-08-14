.syntax unified
.cpu cortex-m4
.fpu softvfp

/* Utilizaremos el set THUMB de instrucciones */
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
	/* Deshabilitamos las interrupciones */
	cpsid	i

	/* Guardamos los valores de los registros */
	mrs	r0, psp
	subs	r0, #16
	stmia	r0!,{r4-r7}
	mov	r4, r8
	mov	r5, r9
	mov	r6, r10
	mov	r7, r11
	subs	r0, #32
	stmia	r0!,{r4-r7}
	subs	r0, #16

	/* Guardamos el Stack del proceso actual  */
	ldr	r2, =os_curr_task
	ldr	r1, [r2]
	str	r0, [r1]

	/* Cargamos el Stack del siguiente proceso  */
	ldr	r2, =os_next_task
	ldr	r1, [r2]
	ldr	r0, [r1]

	/* Colocamos los registros de nuevo*/
	ldmia	r0!,{r4-r7}
	mov	r8, r4
	mov	r9, r5
	mov	r10, r6
	mov	r11, r7
	ldmia	r0!,{r4-r7}
	msr	psp, r0

	/* EXC_RETURN - Thread mode with PSP: */
	ldr r0, =0xFFFFFFFD

	/* Habilitamos las interrupciones  */
	cpsie	i

	bx	r0

.size PendSV_Handler, .-PendSV_Handler
