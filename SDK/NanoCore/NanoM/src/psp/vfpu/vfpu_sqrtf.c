#include "pspmath.h"

float vfpu_sqrtf(float x) 
{
	float result;
	__asm__ volatile (
		"mtv     %1, S700\n"
		"vsqrt.s S000, S000\n"
		"mfv     %0, S700\n"
	: "=r"(result) : "r"(x));
	return result;
}

