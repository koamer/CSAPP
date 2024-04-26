#ifndef TOOLS_H
#define TOOLS_H
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define W_SIZE_IN_BITS_ sizeof(int)<<3
#define BYTE_LENGTH_IN_BITS 8


typedef unsigned char* byte_pointer;
typedef unsigned char byte;

void show_bytes_bits(byte_pointer b_ptr, size_t length);

void show_bytes_bits_float(float x);
void show_bytes_bits_int(int x);
void show_bytes_bits_unsinged(unsigned int x);
void show_bytes_bits_double(double x);

void show_bits(byte b);

void print_sucess(void);
void print_fail(void);

int* create_int_array(int args, ...);

void call_test_int_with_void(int (*funct_ptr)(void), const char* test_name);
void call_test_int_with_int(int (*func_ptr)(int), int* arg_arr, size_t arg_size, int* results_arr, size_t results_size, const char* test_name);
void call_test_bool(bool (*func_ptr)(void), const char* test_name);

#endif
