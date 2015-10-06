/*******************************************************************************
* File Name:		buffer.c
* Compiler:			MS Visual Studio 2013
* Author:			Skye Turriff
* Course:			CST 8152 - Compilers, Lab Section:	011
* Assignment:		1
* Date:				5 October 2015
* Professor:		Sv. Ranev
* Purpose:			Function definitons for prototypes decalred in buffer.h
* Function list:	b_create(), b_addc(), b_reset(), b_destroy(), b_isFull(),
*					b_size(), b_capacity(), b_setmark(), b_mark(), b_mode(),
*					b_inc_factor(), b_load(), b_isempty(), b_eob(), b_getc(),
*					b_print(), b_pack(), b_rflag(), b_retract(),
*					b_retract_to_mark(), b_getc_offset()
*******************************************************************************/

#include <stdio.h>
#include "buffer.h"

/*******************************************************************************
* Purpose:			To allocate memory for one Buffer and initialize members.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		The initial capacity of the buffer as short init_capacity,
*					range 1 to SHRT_MAX inclusive
*					The increment factor of the buffer as char inc_factor, range
*					0 to 255 inclusive
*					The operational mode of the buffer as char o_mode, may be 
*					'a', 'f', or 'm'
* Return value:		On success, a pointer to the Buffer. NULL otherwise.
* Algorithm:		Test for invalid parameters, if found, return NULL. Try to
*					allocate memory for Buffer and character buffer cb_head. If 
*					unsuccessful, return NULL. Set Buffer mode, inc_factor, and
*					capacity members. Return pointer to Buffer pBD.
*******************************************************************************/
Buffer* b_create(short init_capacity, char inc_factor, char o_mode) {

	/* In order to ensure the value of inc_factor is within 0-255, cast 
	to an unsigned char. If function is called with values outside of this 
	range (ie -1, 256), internally the values will be converted to be within 
	UCHAR_MIN and UCHAR_MAX. For example, if 256 is passed as inc_factor, it 
	will be truncated and interpreted by the function as 0. 
	See C99 Standard 6.3.1.3 */
	unsigned char uc_inc_factor = inc_factor;

	/* Test for invalid parameters. */
	if (init_capacity <= 0 ||
		(o_mode != 'a' && o_mode != 'f' && o_mode != 'm') ||
		((o_mode == 'm') && (uc_inc_factor > 100)))	
		return NULL;

	/* Try to allocate memory for one Buffer structure */
	Buffer* pBD = (Buffer *)calloc(1, sizeof(Buffer));

	/* Check for successful allocation */
	if (pBD == NULL)	
		return NULL;

	/* Try to allocate memory for one dynamic character buffer */
	pBD->cb_head = (char *)malloc(init_capacity);

	/* Check for successful allocation */
	if (pBD->cb_head == NULL)	
		return NULL;
	
	/* Set buffer operational mode and increment factor */
	if (uc_inc_factor == 0 || o_mode == 'f') {
		pBD->mode = FIXED;
		pBD->inc_factor = 0;
	}
	else if (o_mode == 'm') {
		pBD->mode = MULTIPLICATIVE;
		pBD->inc_factor = inc_factor;
	}
	else if (o_mode == 'a') {
		pBD->mode = ADDITIVE;
		pBD->inc_factor = inc_factor;
	}

	/* Set buffer starting capacity */
	pBD->capacity = init_capacity;

	return pBD;
}

/*******************************************************************************
* Purpose:			To add a symbol to the character buffer.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	realloc()
* Parameters:		A constant pointer to a valid (non-NULL) Buffer with a valid
*					character buffer cb_head
*					A char symbol to be added to the character buffer
* Return value:		On success, a pointer to the Buffer. NULL otherwise.
* Algorithm:		Check for invalid parameters, if found return NULL. Check if
*					there is room in the character buffer for the symbol, if so
*					add the symbol and return pointer to Buffer pBD. If there is
*					no room and buffer has reached its max size, or if it is in
*					fixed mode, return NULL. Otherwise calculate new capacity
*					and attempt to reallocate space for character buffer. If not
*					successful, return NULL. Otherwise, add symbol.
*******************************************************************************/
pBuffer b_addc(pBuffer const pBD, char symbol) {
	/* Check for operational buffer */
	if (pBD == NULL || pBD->cb_head == NULL)
		return NULL;

	/* If there is room, add the symbol and return */
	if ((pBD->addc_offset*sizeof(char)) < pBD->capacity) {
		pBD->r_flag = 0;
		pBD->cb_head[pBD->addc_offset++] = symbol;
		return pBD;
	}

	/* Check for cases where capacity cannot be increased */
	if (pBD->mode == FIXED || pBD->capacity == MAX_CAPACITY)
		return NULL;

	/* Calculate new capacity of character buffer */
	short new_capacity = 0;
	if (pBD->mode == ADDITIVE)
		new_capacity = pBD->capacity + (unsigned char)pBD->inc_factor;
	if (pBD->mode == MULTIPLICATIVE) {
		short available_space = MAX_CAPACITY - pBD->capacity;
		short new_increment = available_space * pBD->inc_factor / 100;
		new_capacity = pBD->capacity + new_increment;
	}

	/* If new_capacity exceeds max capacity, set to max capacity */
	if (new_capacity == 0)
		return NULL;
	//if (new_capacity <= 0)				
	//	new_capacity = MAX_CAPACITY;
	
	/* Attempt to expand character buffer */
	char* temp_cb_head = (char *)realloc(pBD->cb_head, new_capacity);
	if (temp_cb_head == NULL)
		return NULL;
	/* Check if memory location was changed */
	if (temp_cb_head != pBD->cb_head) {
		pBD->r_flag = SET_R_FLAG;
		pBD->cb_head = temp_cb_head;
	}
	temp_cb_head = NULL;
	/* Add symbol and update capacity */
	pBD->cb_head[pBD->addc_offset++] = symbol;
	pBD->capacity = new_capacity;

	return pBD;
}

/*******************************************************************************
* Purpose:			To reset Buffer members so that its character buffer appears
					to be empty.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer
* Return value:		integer 1 on success, -1 otherwise
*******************************************************************************/
int b_reset(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	/* Reset Buffer members so that character buffer appears empty */
	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->mark_offset = 0;
	pBD->r_flag = 0;
	pBD->eob = 0;

	return SUCCESS;
}

/*******************************************************************************
* Purpose:			To fee dynamically allocated memory used for Buffer
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	free()
* Parameters:		A constant pointer to a Buffer
* Return value:		None
* Algorithm:		Check if character buffer is non-NULL, if so free memory. Do
*					the same for the Buffer struct.
*******************************************************************************/
void b_destroy(Buffer* const pBD) {
	if (pBD->cb_head != NULL)
		free(pBD->cb_head);
	if (pBD != NULL)
		free(pBD);
}

/*******************************************************************************
* Purpose:			To check if the character buffer has reached its current 
*					capacity (is full and has no more room to add symbols).
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer
* Return value:		integer 1 if character buffer is full, 0 if not full, -1 if
*					bad parameters.
*******************************************************************************/
int b_isFull(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	/* Return 1 if character buffer is full, 0 otherwise */
	return pBD->capacity == pBD->addc_offset ? 1 : 0;
}

/*******************************************************************************
* Purpose:			Gets the current occupied size of the character buffer.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer
* Return value:		A short, the number of chars currently held by the buffer,
*					or -1 on bad parameters.
*******************************************************************************/
short b_size(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->addc_offset;
}

/*******************************************************************************
* Purpose:			Gets the current capacity in bytes of the character buffer.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer
* Return value:		A short, the total number of bytes currently allocated to
*					the character buffer. Returns -1 on bad parameters.
*******************************************************************************/
short b_capacity(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;
		
	return pBD->capacity;
}

/*******************************************************************************
* Purpose:			To set the mark location of the character buffer.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer with a valid
*					character buffer cb_head
*					The value to set as the mark location, as short mark, range
*					from 0 to the current size in chars of the character buffer
* Return value:		On success, a pointer to the location in the character
*					buffer where the new mark has been set, otherwise NULL.
*******************************************************************************/
char* b_setmark(Buffer* const pBD, short mark) {
	/* Check for operational buffer and valid mark */
	if (pBD == NULL || pBD->cb_head == NULL ||
		mark < 0 || mark > pBD->addc_offset) 
		return NULL;

	pBD->mark_offset = mark;
	return &(pBD->cb_head[pBD->mark_offset]);
}

/*******************************************************************************
* Purpose:			Gets the value of mark_offset.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer
* Return value:		A short, the value of mark_offset, representing the offset 
*					in chars to the mark location of the character buffer.
*					Returns NULL if bad parameters.
*******************************************************************************/
short b_mark(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->mark_offset;
}

/*******************************************************************************
* Purpose:			Get the operational mode of the Buffer.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer.
* Return value:		An integer 0 if the buffer is in fixed mode, -1 if it is in
*					multiplicative mode, or 1 if in additive mode. Returns -2
*					if bad parameters.
*******************************************************************************/
int b_mode(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_2;

	return pBD->mode;
}

/*******************************************************************************
* Purpose:			Gets the increment factor of the Buffer.
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:		A constant pointer to a valid (non-NULL) Buffer.
* Return value:		On success, returns size_t representing the increment factor 
*					of the buffer from 0-255 inclusive. Returns 256 on error.
*******************************************************************************/
size_t b_inc_factor(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_256;

	/*Return NON NEGATIVE VALUE of inc_factor*/
	return (unsigned char)pBD->inc_factor; 
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
int b_load(FILE* const fi, Buffer* const pBD) {
	/* Check for operational buffer and file */
	if (pBD == NULL || fi == NULL)
		return R_FAIL_1;

	int numAdded = 0;
	int ch = fgetc(fi);
	while (!feof(fi)){
		if (!b_addc(pBD, ch))
			return LOAD_FAIL;
		numAdded++;
		ch = fgetc(fi);
	}

	return numAdded;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
int b_isempty(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->addc_offset == 0 ? 1 : 0;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
int b_eob(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->eob;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
char b_getc(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL || pBD->cb_head == NULL) 
		return R_FAIL_2;

	if (pBD->getc_offset == pBD->addc_offset) {
		pBD->eob = SET_R_FLAG;
		return R_FAIL_1;
	}

	pBD->eob = 0;
	return pBD->cb_head[pBD->getc_offset++];
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
int b_print(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL || pBD->cb_head == NULL) 
		return R_FAIL_1;

	int numChars = 0;
	if (pBD->addc_offset == 0)
		printf("The buffer is empty.\n");
	else {
		pBD->getc_offset = 0;
		char ch = b_getc(pBD);
		while (!b_eob(pBD)){
			printf("%c", ch);
			numChars++;
			ch = b_getc(pBD);
		}
		pBD->getc_offset = 0;
		pBD->eob = 0;
		printf("\n");
	}

	return numChars;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
Buffer *b_pack(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL || pBD->cb_head == NULL)
		return NULL;

	/* Check for valid new capacity */
	short new_capacity = (pBD->addc_offset + 1)*sizeof(char);
	if (new_capacity <= 0)
		return NULL;
	
	/* Try to allocate space */
	char* tempB = (char *)realloc(pBD->cb_head, new_capacity);
	if (tempB == NULL)
		return NULL;

	/* Check if location in memory has changed */
	if (tempB != pBD->cb_head) {
		pBD->r_flag = SET_R_FLAG;
		pBD->cb_head = tempB;
	}

	tempB = NULL;
	pBD->capacity = new_capacity;
	return pBD;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
char b_rflag(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->r_flag;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
short b_retract(Buffer* const pBD) {
	/* Check for operational buffer and positive getc_offset */
	if (pBD == NULL || pBD->getc_offset <= 0) 
		return R_FAIL_1;

	return --(pBD->getc_offset);
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
short b_retract_to_mark(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->getc_offset = pBD->mark_offset;
}

/*******************************************************************************
* Purpose:
* Author:			Skye Turriff
* History:			Version 1, October 5, 2015
* Called functions:	None
* Parameters:
* Return value:
* Algorithm:
*******************************************************************************/
short b_getc_offset(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->getc_offset;
}