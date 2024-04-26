#include <stdlib.h>
#include <string.h>


#include "tools.h"

void print_fail(void) {
	printf("\033[0;31mFAIL\n");
	printf("\033[m \n");
	exit(EXIT_FAILURE);
}

void print_sucess(void) {
	printf("\033[0;32mSUCCESS\n");
	printf("\033[m \n");
}

void show_bytes_bits(byte_pointer b_ptr, size_t length) {
        for(size_t i = 0; i < length; i++) {
                printf("%x ", b_ptr[i]);
		show_bits(b_ptr[i]);
		puts("\n");
        }
}

void show_bytes_bits_float(float x) {
	show_bytes_bits((byte_pointer)&x, sizeof(float));
}

void show_bytes_bits_int(int x) {
	show_bytes_bits((byte_pointer)&x, sizeof(int));
}

void show_bytes_bits_unsinged(unsigned int x) {
	show_bytes_bits((byte_pointer)&x, sizeof(unsigned int));
}

void show_bytes_bits_double(double x) {
	show_bytes_bits((byte_pointer)&x, sizeof(double));
}

void show_bytes_bits_short(short x) {
	show_bytes_bits((byte_pointer)&x, sizeof(short));
}

void show_bytes_bits_long(long x) {
	show_bytes_bits((byte_pointer)&x, sizeof(long));
}

void show_bits(byte b) {
	for(int mask = 128; mask >= 1; mask >>= 1) {
		char ch = b & mask ? '1': '0';
		printf("%c", ch);
	}
}

void call_test_int_with_void(int (*func_ptr)(void), const char* test_name) {
	puts(test_name);
	if(func_ptr() == 1) {
		print_sucess();
	}
	else {
		print_fail();
	}
}

void call_test_int_with_int(int (*func_ptr)(int), int* arg_arr, size_t arg_size, int* results_arr, size_t results_size, const char* test_name) {

	puts(test_name);

	if(arg_size != results_size) {
		exit(EXIT_FAILURE);
	}
	for(size_t i = 0; i < arg_size; i++) {
		if (func_ptr(arg_arr[i]) != results_arr[i]) {
			printf("Got: %d Expected: %d\n",func_ptr(arg_arr[i]), results_arr[i]);
			print_fail();
		}
	}
	print_sucess();

	free(arg_arr);
	free(results_arr);
}

void call_test_bool(bool (*func_ptr)(void), const char* test_name) {
	puts(test_name);
	if(func_ptr() == true) {
		print_sucess();
	}
	else {
		print_fail();
	}
}


int* create_int_array(int args, ...) {
	va_list list;
	size_t number_of_arguments = args;
	va_start(list, args);

	int* array = malloc(number_of_arguments * sizeof(int));

	memset(array, 0, number_of_arguments * sizeof(int));

	for(size_t i = 0; i < number_of_arguments; i++){
		array[i] = va_arg(list, int);
	}
	va_end(list);
	
	return array;
}