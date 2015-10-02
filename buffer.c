#include <stdio.h>
#include <limits.h>
#include "buffer.h"


Buffer* b_create(short init_capacity, char inc_factor, char o_mode) {

	/* In order to ensure the value of inc_factor is within 0-255, cast 
	to an unsigned char. If function is called with values outside of this 
	range (ie -1, 256), internally the values will be converted to be within 
	UCHAR_MIN and UCHAR_MAX. For example, if 256 is passed as inc_factor, it 
	will be truncated and interpreted by the function as 0. 
	See C99 Standard 6.3.1.3 */
	unsigned char uc_inc_factor = inc_factor;

	/* Using Svillen's test for valid o_mode */
	switch (o_mode) {	
	case 'a': 
	case 'f': 
	case 'm':
		break;
	default:
		return NULL;
	}

	/* Test for valid init_capacity and inc_factor arguments.
	If a number greater than SHRT_MAX is passed for init_capacity, overflow 
	will cause variable to become negative. */
	if (init_capacity <= 0 ||  ((o_mode == 'm') && (uc_inc_factor > 100)))	
		return NULL;

	/* All arguments valid. Try to allocate memory for one Buffer structure */
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
		pBD->mode = 0;
		pBD->inc_factor = 0;
	}
	else if (o_mode == 'm') {
		pBD->mode = -1;
		pBD->inc_factor = inc_factor;
	}
	else if (o_mode == 'a') {
		pBD->mode = 1;
		pBD->inc_factor = inc_factor;
	}

	/* Set buffer starting capacity */
	pBD->capacity = init_capacity;

	return pBD;
}

pBuffer b_addc(pBuffer const pBD, char symbol) {

	/* Check for operational buffer */
	if (pBD == NULL || pBD->cb_head == NULL)
		return NULL;

	/* If there is room, add the symbol and return */
	if (pBD->addc_offset < pBD->capacity) {
		pBD->r_flag = 0;
		pBD->cb_head[pBD->addc_offset++] = symbol;
		return pBD;
	}

	/* If buffer is full and in fixed mode, or if maximum buffer size has 
	been reached, return NULL*/
	if (pBD->mode == 0 || pBD->capacity == MAX_CAPACITY)
		return NULL;

	/* Calculate new capacity of character buffer */
	short new_capacity;
	if (pBD->mode == 1)
		new_capacity = pBD->capacity + pBD->inc_factor;
	if (pBD->mode == -1) {
		short available_space = MAX_CAPACITY - pBD->capacity;
		short new_increment = available_space * pBD->inc_factor / 100;
		new_capacity = pBD->capacity + new_increment;
	}

	/* If new capacity does not exceed max capacity of the buffer, increment 
	current capacity. Else, set current capacity to max capacity */
	if (new_capacity > 0 && new_capacity <= MAX_CAPACITY)	/*may not need to test for <= max*/
		pBD->capacity = new_capacity;
	else {
		pBD->capacity = MAX_CAPACITY;
	
	/* Attempt to expand character buffer */
	char* temp_cb_head = (char *)realloc(pBD->cb_head, pBD->capacity);
}

int b_reset(Buffer* const pBD) {

}

void b_destroy(Buffer* const pBD) {
	free(pBD->cb_head);
	free(pBD);
}

int b_isFull(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->capacity == pBD->addc_offset ? 0 : 1;
}

short b_size(Buffer* const pBD) {
	/*Check fro run-time errors. If an error occurs, RETURN -1*/

	return pBD->addc_offset;
}

short b_capacity(Buffer* const pBD) {
	/*Check for run-time errors. If an erro occurs, RETURN -1*/
		
	return pBD->capacity;
}

char* b_setmark(Buffer* const pBD, short mark) {
	/*If funtion cannot complete task (run-time error), RETURN NULL*/

	if (0 <= mark && mark <= pBD->addc_offset){
		pBD->mark_offset = mark;
		return &(pBD->cb_head[pBD->mark_offset]);
	}
	else
		return NULL;
}

short b_mark(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->mark_offset;
}

int b_mode(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, notify the calling function of the error*/

	return pBD->mode;
}

size_t b_inc_factor(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN 256*/

	return (unsigned char)pBD->inc_factor; /*Return NON_NEGATIVE VALUE of inc_factor*/
}

int b_load(FILE* const fi, Buffer* const pBD) {
	return R_FAIL_1;
}

int b_isempty(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->addc_offset == 0 ? 1 : 0;
}

int b_eob(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->eob;
}

char b_getc(Buffer* const pBD) {
	/*Check argument for validity (possible run-time error). 
	If invalid, RETURN -2*/

	if (pBD->getc_offset == pBD->addc_offset) {
		pBD->eob = 1;
		return -1;
	}
	else {
		pBD->eob = 0;
		char ch = pBD->cb_head[pBD->getc_offset];
		pBD->getc_offset++;
		return ch;
	}
}

int b_print(Buffer* const pBD) {

}

Buffer *b_pack(Buffer* const pBD) {
	
}

char b_rflag(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->r_flag;
}

short b_retract(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return --(pBD->getc_offset);
}

short b_retract_to_mark(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->getc_offset = pBD->mark_offset;
}

short b_getc_offset(Buffer* const pBD) {
	/*Check for run-time errors. If an error occurs, RETURN -1*/

	return pBD->getc_offset;
}