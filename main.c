#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

int bit_level_is_equal(int x, int y) {
	return !(x^y);
}

int uadd_ok(unsigned int x, unsigned int y) {
	return !(x + y < x);
}

int tadd_ok(int x, int y) {
	int positive_overflow = (x > 0) && (y > 0) && (x + y) >= 0;
	int negative_overflow = (x < 0) && (y < 0) && (x + y) <= 0;
	return positive_overflow || negative_overflow;
}

int tmult_ok(int x, int y) {
	int64_t res = (int64_t) x * y;
	return res == (int)res;
}

int div16(int x) {
	int bias = (x >> 31) & 0xf;
	return (x + bias) >> 4;
}
float create_float_from_binary(void) {
	// -3
	float x = 0.0;
	int s =  INT_MIN; // << 31;
	// int exp = 1 << 23;
	return (float)s;
}

bool test_bit_level_is_equal(void) {
	return (bit_level_is_equal(3, 3) && (!bit_level_is_equal(5, 3)));
}

bool test_uadd_ok(void) {
	return ((uadd_ok(13, 15)) && (uadd_ok(0xffff, 0xA)) &&
		(!uadd_ok(UINT_MAX, 1)));
}

bool test_tadd_ok(void) {
	return ((tadd_ok(13, 15)) && (!tadd_ok(INT_MIN, -2)) &&
 		(!tadd_ok(INT_MAX - 1, 2)));
}

bool test_tmult_ok(void) {
	return (tmult_ok(16, 32) && tmult_ok(-5 ,3) &&
		!tmult_ok(INT_MAX, 2));
}

bool test_div16(void){
	return ((div16(48) == 3) && (div16(-32) == -2)  &&
		(div16(24) == 1) && (div16(-35) == -2));
}

inline static void print_sucess(void) {
	printf("\033[0;32mSUCCESS\n");
	printf("\033[m \n");
}

inline static void print_fail(void) {
	printf("\033[0;31mFAIL\n");
	printf("\033[m \n");
}

void RUN_ALL_TESTS(void)  {
	puts("Bit lvevl equality... ");
	if (test_bit_level_is_equal() == true) {
		print_sucess();
	}
	else {
		print_fail();
		exit(EXIT_FAILURE);
	}

	puts("Unsigned addition overflow check... ");
	if(test_uadd_ok() == true) {
		print_sucess();
	}
	else {
		print_fail();
		exit(EXIT_FAILURE);
	}

	puts("Signed addition overflow check...  ");
	if (test_tadd_ok() == true) {
		print_sucess();
	}
	else {
		print_fail();
		exit(EXIT_FAILURE);
	}
	puts("Signed multiplication overflow check...  ");
	if(test_tmult_ok() == true) {
		print_sucess();
	}
	else {
		print_fail();
		exit(EXIT_FAILURE);
	}
	puts("Division by 16 check...  ");
	if(test_div16() == true) {
		print_sucess();
	}
	else {
		print_fail();
		exit(EXIT_FAILURE);
	}
	printf("Created float %f\n",create_float_from_binary());
}

int main(void) {
	RUN_ALL_TESTS();
	return 0;
}
