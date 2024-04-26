#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include "tools.h"

static const int W_SIZE_IN_BITS = W_SIZE_IN_BITS_;

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

int is_little_endian(void) {
	int x = 0xabffde;
	byte_pointer d = (byte_pointer)&x;
	return (d[0] & 0xff) == 0xde;
}

int replacing_least_significant_byte(int x, int y) {
	int least_byte_x = x & 0xff;
	int y_without_least_byte = y & (~0xff);
	return least_byte_x + y_without_least_byte;
}

// 2.60
unsigned int replace_byte(unsigned int x, int i, byte b) {
	int mask = ~(0xff << (8 * i)); 
	return x & mask | (b << (8 * i));
}

// 2.61A
int check_if_any_bit_is_1(int x) {
	return !(x == 0);
}

// 2.61B
int check_if_any_bit_is_0(int x) {
	int bit_mask = -1; // -1 is a value of all ones in 32 bits (is equal to (~0x00) expression)
	return (x & bit_mask) != bit_mask;
}

//2.61C
int any_bit_in_least_significant_bytes_is_1(int x) {
	return !((x & 0xff) == 0);
}

//2.61D
int any_bit_in_most_significant_bytes_is_0(int x) {
	unsigned int mask =  0xff << ((W_SIZE_IN_BITS) - BYTE_LENGTH_IN_BITS); // all bits in most significant bytes sets to 1 
	return !((x & mask) == mask); 
}

//2.62
int int_shifts_are_arithmetic(void) {
	int bit_mask = -1; // regardless of word size will alwatys has all one's
	return !(bit_mask ^ (bit_mask >> 1));
}

//2.63
unsigned int srl(unsigned int x, int k) {
	unsigned int xsra = (int) x >> k;

	int mask = (int) -1 << (W_SIZE_IN_BITS - k);

	return xsra & ~mask;
}

int sra(int x, int k) {
	int xsrl = (unsigned) x >> k;

	int mask = (int) -1 << (W_SIZE_IN_BITS - k);
	
	int m = 1 << (W_SIZE_IN_BITS - 1);

	mask &= ! (x & m) - 1;

	return xsrl | mask;
}

int any_odd_one(int x) {
	return !(!(0xAAAAAAAA & x));
}

int odd_ones(int x) {
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	x &= 0x1;
	return x;
}

int leftmost_one(int x) {

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

bool test_replacing_least_significant_byte(void) {
	return replacing_least_significant_byte(0x89ABCDEF, 0x76543210) == 0x765432EF;
}

bool test_replace_byte(void) {
	return (replace_byte(0x12345678, 2, 0xAB) == 0x12AB5678) && (replace_byte(0x12345678, 0, 0xAB) == 0x123456AB);
}

bool test_srl(void) {
	return (srl(3, 1) == 1 ) && (srl(-1, 1) == 0x7FFFFFFF );
}

bool test_sra(void) {
	return (sra(3, 1) == 1 ) && (sra(-1, 2) == 0xFFFFFFFF );
}

void RUN_ALL_TESTS(void)  {
	call_test_bool(test_bit_level_is_equal, "Bit lvevl equality... ");

	call_test_bool(test_uadd_ok, "Unsigned addition overflow check... ");

	call_test_bool(test_tadd_ok, "Signed addition overflow check...  ");

	call_test_bool(test_tmult_ok, "Signed multiplication overflow check...  ");

	call_test_int_with_int(div16, create_int_array(4, 48, -32, 24, -35), 4, create_int_array(4, 3, -2, 1, -2), 4, "Division by 16 check...  ");

	call_test_int_with_void(is_little_endian, "Is little ednian check ...  ");

	call_test_bool(test_replacing_least_significant_byte, "Change Least significant byte ...  ");

	call_test_bool(test_replace_byte, "Replace a byte ....  ");

	call_test_int_with_int(check_if_any_bit_is_1, create_int_array(3, 1, 0, 10), 3, create_int_array(3, 1, 0, 1), 3, "Check if any bit is set to 1 .... ");

	call_test_int_with_int(check_if_any_bit_is_0, create_int_array(3, -1, 1, 0), 3, create_int_array(3, 0, 1, 1), 3, "Check if any bit is set to 0 .... ");

	call_test_int_with_int(any_bit_in_least_significant_bytes_is_1, create_int_array(3, 0 , 0xff, 3), 3, create_int_array(3, 0, 1, 1), 3, 
						   "Check if any bit in least significant byte is set to 1 ...");

	call_test_int_with_int(any_bit_in_most_significant_bytes_is_0, create_int_array(4, 0 , 0xff, INT_MAX, -1), 4, create_int_array(4, 1, 1, 1, 0), 4, 
						   "Check if any bit in most significant byte is set to 0  ...");

	call_test_int_with_void(int_shifts_are_arithmetic, "Check if right shift are arithmetic ... ");
	
	call_test_bool(test_srl, "Test logical right shift .... ");

	call_test_bool(test_sra,"Test arithmetic right shift .... ");

	call_test_int_with_int(any_odd_one, create_int_array(2, 2, 1), 2, create_int_array(2, 1, 0), 2, "Check if any odd bits is enabled ....");	

	call_test_int_with_int(odd_ones, create_int_array(2, 0x10101011, 0x01010101), 2, create_int_array(2, 1, 0), 2, "Check if contains an odd number of 1s ... ");
 }

int main(void) {
	RUN_ALL_TESTS();
	return 0;
}
