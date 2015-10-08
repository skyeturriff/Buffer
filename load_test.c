/*******************************************************************************
* File Name:		load_test.c
* Compiler:			MS Visual Studio 2013
* Author:			Skye Turriff, using code from platy_bt.c written by Sv. Ranev
* Course:			CST 8152 - Compilers, Lab Section:	011
* Assignment:		1
* Date:				5 October 2015
* Professor:		Sv. Ranev
* Purpose:			To test the b_load() method of buffer.c
* Function list:	fopen(), printf(), b_load(), fclose(), b_capacity(), 
*					b_size(), b_mode(), b_inc_factor(), b_mark(), b_print()
*******************************************************************************/

#include <stdio.h>
#include "buffer.h"

int main()
{
	FILE *fp;
	int loadsize = 0;
	pBuffer ptr_Buffer;

	ptr_Buffer = b_create(200, 15, 'f');

	fp = fopen("Test.txt", "r");
	if (fp == NULL)
	{
		printf("Error in opening file");
		return(-1);
	}

	/* load a source file into the input buffer  */
	printf("Reading file Test.txt ....Please wait\n");
	loadsize = b_load(fp, ptr_Buffer);
	if (loadsize == R_FAIL_1)
		printf("Error in loading buffer.\n");
	else
		printf("number of symbols loaded: %d\n", loadsize);

	fclose(fp);

	printf("\nPrinting buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", b_capacity(ptr_Buffer));
	printf("The current size of the buffer is:  %d\n", b_size(ptr_Buffer));
	printf("The operational mode of the buffer is:   %d\n", b_mode(ptr_Buffer));
	printf("The increment factor of the buffer is:  %u\n", b_inc_factor(ptr_Buffer));
	printf("The current mark of the buffer is:  %d\n", b_mark(ptr_Buffer));
	/*printf("The reallocation flag is:   %d\n",b_rflag(ptr_Buffer));*/
	printf("\nPrinting buffer contents:\n\n");
	b_print(ptr_Buffer);

	return(0);
}