#include "BitController.h"

void BitAddition(unsigned char* _lookat, const unsigned char _plus)
{
	*_lookat |= _plus;
}

void BitTakeaway(unsigned char* _lookat, const unsigned char _negative)
{
	unsigned int mask = ~_negative;
	*_lookat &= mask;
}

bool BitChecker(const unsigned char _lookat, const unsigned char _check)
{
	if ((_lookat&_check) == _check) return true;
	else return false;
}