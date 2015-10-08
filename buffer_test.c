/*******************************************************************************
* File Name:		buffer_test.c
* Compiler:			MS Visual Studio 2013
* Author:			Skye Turriff, using code from platy_bt.c written by Sv. Ranev
* Course:			CST 8152 - Compilers, Lab Section:	011
* Assignment:		1
* Date:				5 October 2015
* Professor:		Sv. Ranev
* Purpose:			To test various functions implemented in buffer.c
* Function list:	b_create(), b_addc(), printf(), b_capacity(), b_size(), 
*					b_getc_offset(), b_mode(), b_inc_factor(), b_mark(), 
*					b_print(), b_pack(), b_isFull(), b_set_mark() 
*******************************************************************************/

#include "buffer.h"

int main(int argc, char **argv) {
	Buffer *ptr_Buffer;   /* pointer to Buffer structure */
	/*ptr_Buffer = b_create(*argv[1], *argv[2], *argv[3]);*/
	ptr_Buffer = b_create(1, 15, 'a');

	/*loop used to fill character buffer to its capacity*/
	int i = 0;
	while (i < MAX_CAPACITY){
		b_addc(ptr_Buffer, 'a');
		i++;
	}
	//while (i < MAX_CAPACITY){
	//	b_addc(ptr_Buffer, 'a');
	//	i++;
	//}
	//b_addc(ptr_Buffer, 'a');
	//b_addc(ptr_Buffer, 'a');
	printf("\nPrinting buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", /*ptr_Buffer->capacity*/ b_capacity(ptr_Buffer));
	printf("The current size of the buffer is:  %d\n", /*ptr_Buffer->addc_offset*/ b_size(ptr_Buffer));
	printf("The get char offset of the buffer is:  %d\n", /*ptr_Buffer->addc_offset*/ b_getc_offset(ptr_Buffer));
	printf("The operational mode of the buffer is:   %d\n", /*ptr_Buffer->mode*/ b_mode(ptr_Buffer));
	printf("The increment factor of the buffer is:  %u\n", /*ptr_Buffer->inc_factor*/ b_inc_factor(ptr_Buffer));
	printf("The current mark of the buffer is:  %d\n", /*ptr_Buffer->mark_offset*/ b_mark(ptr_Buffer));
	/*printf("The reallocation flag is:   %d\n",b_rflag(ptr_Buffer));*/
	printf("\nPrinting buffer contents:\n\n");
	b_print(ptr_Buffer);

	/*test b_pack()*/
	b_pack(ptr_Buffer);
	b_addc(ptr_Buffer, 'F');
	b_addc(ptr_Buffer, 'N');
	b_pack(ptr_Buffer);

	/*test b_isfull()*/
	printf("Buffer contents:\n");
	for (i = 0; i < 40; i++)
		printf("%c", ptr_Buffer->cb_head[i]);
	printf("\n");
	if (b_isFull(ptr_Buffer))
		printf("Buffer is full\nCapacity: %d size: %d\n\n", ptr_Buffer->capacity, b_size(ptr_Buffer));
	else 
		printf("Buffer is not full\nCapacity: %d size: %d\n\n", ptr_Buffer->capacity, b_size(ptr_Buffer));

	printf("Adding one more char\n\n");
	b_addc(ptr_Buffer, 'a');

	/*test b_isfull() again*/
	printf("Buffer contents:\n");
	for (i = 0; i < 25; i++)
		printf("%c", ptr_Buffer->cb_head[i]);
	printf("\n");
	if (b_isFull(ptr_Buffer))
		printf("Buffer is full\nCapacity: %d size: %d\n\n", ptr_Buffer->capacity, b_size(ptr_Buffer));
	else
		printf("Buffer is not full\nCapacity: %d size: %d\n\n", ptr_Buffer->capacity, b_size(ptr_Buffer));

	/*test b_getc()*/
	printf("Getting chars from character buffer using b_getc():\n");
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("%c", b_getc(ptr_Buffer));
	printf("\n\n");

	/*test b_setmark()*/
	printf("Testing b_setmark()\n");
	printf("ptr_Buffer mark_offset set to: %d\n", b_mark(ptr_Buffer));
	b_setmark(ptr_Buffer, 5);
	printf("ptr_Buffer mark_offset set to: %d\n\n", b_mark(ptr_Buffer));
	
	/*test b_reset()
	printf("Resetting buffer and adding 3 symbols DDD\n");
	b_reset(ptr_Buffer);
	b_addc(ptr_Buffer, 'D'); b_addc(ptr_Buffer, 'D'); b_addc(ptr_Buffer, 'D');
	printf("Accessing over-written character buffer contents\n");
	for (i = 0; i < 15; i++)
		printf("%c", ptr_Buffer->cb_head[i]);
	printf("\n");
	*/

	/*
	printf("capacity: %d\n", *argv[1]);
	printf("inc_factor char: %c\tinc_factor int: %d\n", *argv[2], *argv[2]);
	printf("mode car: %c\n", argv[3]);
	*/
	if (ptr_Buffer == NULL)
		printf("Could not create buffer\n");
	else {
		printf("\nPrinting buffer parameters:\n\n");
		printf("The capacity of the buffer is:  %d\n", /*ptr_Buffer->capacity*/ b_capacity(ptr_Buffer));
		printf("The current size of the buffer is:  %d\n", /*ptr_Buffer->addc_offset*/ b_size(ptr_Buffer));
		printf("The get char offset of the buffer is:  %d\n", /*ptr_Buffer->addc_offset*/ b_getc_offset(ptr_Buffer));
		printf("The operational mode of the buffer is:   %d\n", /*ptr_Buffer->mode*/ b_mode(ptr_Buffer));
		printf("The increment factor of the buffer is:  %u\n", /*ptr_Buffer->inc_factor*/ b_inc_factor(ptr_Buffer));
		printf("The current mark of the buffer is:  %d\n", /*ptr_Buffer->mark_offset*/ b_mark(ptr_Buffer));
		/*printf("The reallocation flag is:   %d\n",b_rflag(ptr_Buffer));*/
		printf("\nPrinting buffer contents:\n\n");
		b_print(ptr_Buffer);
	}

	return 0;
}