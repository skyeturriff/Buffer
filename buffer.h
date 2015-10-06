/*******************************************************************************
* File Name:		buffer.h
* Compiler:			MS Visual Studio 2013
* Author:			S^R
* Course:			CST 8152 - Compilers, Lab Section:	011 
* Assignment:		1
* Date:				5 October 2015
* Professor:		Sv. Ranev
* Purpose:			Preprocessor directives, type declarations and prototypes 
*					necessary for buffer implementation as required for CST8152, 
*					Assignment #1, Fall 2015.
* Function list:	b_create(), b_addc(), b_reset(), b_destroy(), b_isFull(),
*					b_size(), b_capacity(), b_setmark(), b_mark(), b_mode(),
*					b_inc_factor(), b_load(), b_isempty(), b_eob(), b_getc(),
*					b_print(), b_pack(), b_rflag(), b_retract(),
*					b_retract_to_mark(), b_getc_offset()	
*******************************************************************************/

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>		/* standard input/output */
#include <malloc.h>		/* for dynamic memory allocation*/
#include <limits.h>		/* implementation-defined data type ranges and limits */

/* constant definitions */
#define SUCCESS 1				/* success return value */
#define R_FAIL_1 -1				/* fail return value */
#define R_FAIL_2 -2				/* fail return value */
#define R_FAIL_256 256			/* fail return value */
#define LOAD_FAIL -2			/* load fail error */
#define SET_R_FLAG 1			/* realloc flag set value */
#define MAX_CAPACITY SHRT_MAX	/* max size allowed for character buffer */
#define FIXED 0					/* fixed buffer mode */
#define ADDITIVE 1				/* additive buffer mode */
#define MULTIPLICATIVE -1		/* multiplicative buffer mode */


/* user data type declarations */
typedef struct BufferDescriptor {
	char *cb_head;		/* pointer to the beginning of character array (character buffer) */
	short capacity;		/* current dynamic memory size (in bytes) allocated to character buffer */
	short addc_offset;  /* the offset (in chars) to the add-character location */
	short getc_offset;  /* the offset (in chars) to the get-character location */
	short mark_offset;	/* the offset (in chars) to the mark location */
	char  inc_factor;	/* character array increment factor */
	char  r_flag;		/* character array reallocation flag */
	char  mode;			/* operational mode indicator*/
	int   eob;			/* end-of-buffer reached flag */
} Buffer, *pBuffer;

/*typedef Buffer *pBuffer;*/

/* function declarations */
Buffer* b_create(short, char, char);
pBuffer b_addc(pBuffer const, char);
int b_reset(Buffer* const);
void b_destroy(Buffer* const);
int b_isFull(Buffer* const);
short b_size(Buffer* const);
short b_capacity(Buffer* const);
char* b_setmark(Buffer* const, short);
short b_mark(Buffer* const);
int b_mode(Buffer* const);
size_t b_inc_factor(Buffer* const);
int b_load(FILE* const, Buffer* const);
int b_isempty(Buffer* const);
int b_eob(Buffer* const);
char b_getc(Buffer* const);
int b_print(Buffer* const);
Buffer *b_pack(Buffer* const);
char b_rflag(Buffer* const);
short b_retract(Buffer* const);
short b_retract_to_mark(Buffer* const);
short b_getc_offset(Buffer* const);

#endif
