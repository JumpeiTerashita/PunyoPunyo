#include "BitCalc.h"


int ColChanger(COLORPATTERN _color)
{
	return (24 - 8 * _color);
}

unsigned int BitColPicker(COLORPATTERN _color,const unsigned int _lookat)
{
	unsigned int mask = 255 << ColChanger(_color);
	unsigned int picked = (_lookat & mask) >> ColChanger(_color);
	return picked;
}

unsigned int AddBit(COLORPATTERN _color,const unsigned int _lookat,const unsigned int _add)
{
	unsigned int result = 0;
	result = _lookat + (_add << ColChanger(_color));
	return result;
}