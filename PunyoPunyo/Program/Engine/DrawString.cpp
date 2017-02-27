#include "DrawString.h"
#include "../glut.h"

void DrawString(const char* str)
{
	while (*str != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str++);
	}
}

void DrawString_Stroke(const char* str)
{
	while (*str != '\0')
	{
		glutStrokeCharacter(
			GLUT_STROKE_ROMAN,  // void *font
			*str++);               // int character
	}
}

