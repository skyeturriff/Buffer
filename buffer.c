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
	been reached, return NULL */
	if (pBD->mode == 0 || pBD->capacity == MAX_CAPACITY)
		return NULL;

	/* Calculate new capacity of character buffer */
	short new_capacity = 0;
	if (pBD->mode == 1)
		new_capacity = pBD->capacity + pBD->inc_factor;
	if (pBD->mode == -1) {
		short available_space = MAX_CAPACITY - pBD->capacity;
		short new_increment = available_space * pBD->inc_factor / 100;
		new_capacity = pBD->capacity + new_increment;
	}

	/*NOTE - IF new_capacity is never set... ie if statements are not entered because
	mode is not reliable... default behaviour will be to set capacity to max size.
	Maybe, if new_capacity still = 0, return NULL?*/

	/* If new_capacity exceeds max capacity, set to max capacity */
	if (new_capacity <= 0)				
		new_capacity = MAX_CAPACITY;
	
	/* Attempt to expand character buffer */
	char* temp_cb_head = (char *)realloc(pBD->cb_head, new_capacity);
	if (temp_cb_head == NULL)
		return NULL;
	/* Check if memory location was changed */
	if (temp_cb_head != pBD->cb_head)
		pBD->r_flag = SET_R_FLAG;
	/* Reassign character buffer to point to new memory, and add symbol */		/* Free temp buffer here??? Double check addc_offset < capacity??? */
	pBD->cb_head = temp_cb_head;
	pBD->cb_head[pBD->addc_offset++] = symbol;
	pBD->capacity = new_capacity;
	return pBD;
}

int b_reset(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->mark_offset = 0;
	pBD->r_flag = 0;
	pBD->eob = 0;

	return 1;		/*Not sure what to return here*/
}

void b_destroy(Buffer* const pBD) {
	if (pBD->cb_head != NULL)
		free(pBD->cb_head);
	if (pBD != NULL)
		free(pBD);
}

int b_isFull(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	/* Return 1 if character buffer is full, 0 otherwise */
	return pBD->capacity == pBD->addc_offset ? 1 : 0;
}

short b_size(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->addc_offset;
}

short b_capacity(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;
		
	return pBD->capacity;
}

char* b_setmark(Buffer* const pBD, short mark) {
	/* Check for operational buffer and valid mark */
	if (pBD == NULL || mark < 0 || mark > pBD->addc_offset) 
		return NULL;

	pBD->mark_offset = mark;
	return &(pBD->cb_head[pBD->mark_offset]);
}

short b_mark(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->mark_offset;
}

int b_mode(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->mode;
}

size_t b_inc_factor(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_256;

	return (unsigned char)pBD->inc_factor; /*Return NON_NEGATIVE VALUE of inc_factor*/
}

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

int b_isempty(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->addc_offset == 0 ? 1 : 0;
}

int b_eob(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->eob;
}

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
		free(pBD->cb_head);
		pBD->cb_head = tempB;
	}

	tempB = NULL;
	pBD->capacity = new_capacity;
	return pBD;
}

char b_rflag(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->r_flag;
}

short b_retract(Buffer* const pBD) {
	/* Check for operational buffer and positive getc_offset */
	if (pBD == NULL || pBD->getc_offset <= 0) 
		return R_FAIL_1;

	return --(pBD->getc_offset);
}

short b_retract_to_mark(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->getc_offset = pBD->mark_offset;
}

short b_getc_offset(Buffer* const pBD) {
	/* Check for operational buffer */
	if (pBD == NULL) return R_FAIL_1;

	return pBD->getc_offset;
}