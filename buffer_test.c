#include "buffer.h"

int main(int argc, char **argv) {
	Buffer *ptr_Buffer;   /* pointer to Buffer structure */
	/*ptr_Buffer = b_create(*argv[1], *argv[2], *argv[3]);*/
	ptr_Buffer = b_create(32700, 100, 'a');

	/*loop used to fill character buffer to its capacity*/
	int i = 0;
	while (i < 32700){
		b_addc(ptr_Buffer, 'a');
		i++;
	}
	b_addc(ptr_Buffer, 'a');
	while (i < MAX_CAPACITY){
		b_addc(ptr_Buffer, 'a');
		i++;
	}
	b_addc(ptr_Buffer, 'a');
	b_addc(ptr_Buffer, 'a');

	/*
	printf("capacity: %d\n", *argv[1]);
	printf("inc_factor char: %c\tinc_factor int: %d\n", *argv[2], *argv[2]);
	printf("mode car: %c\n", argv[3]);
	*/
	if (ptr_Buffer == NULL)
		printf("Could not create buffer\n");
	else {
		printf("capacity + inc_factor: %d\n", ptr_Buffer->capacity + ptr_Buffer->inc_factor);
		printf("capacity + us_inc_factor: %d\n", ptr_Buffer->capacity + (int)ptr_Buffer->inc_factor);
		printf("\nPrinting buffer parameters:\n\n");
		printf("The capacity of the buffer is:  %d\n", /*ptr_Buffer->capacity*/ b_capacity(ptr_Buffer));
		printf("The current size of the buffer is:  %d\n", /*ptr_Buffer->addc_offset*/ b_size(ptr_Buffer));
		printf("The operational mode of the buffer is:   %d\n", /*ptr_Buffer->mode*/ b_mode(ptr_Buffer));
		printf("The increment factor of the buffer is:  %u\n", /*ptr_Buffer->inc_factor*/ b_inc_factor(ptr_Buffer));
		printf("The current mark of the buffer is:  %d\n", /*ptr_Buffer->mark_offset*/ b_mark(ptr_Buffer));
		/*printf("The reallocation flag is:   %d\n",b_rflag(ptr_Buffer));*/
		printf("\nPrinting buffer contents:\n\n");
		b_print(ptr_Buffer);
	}

	return 0;
}