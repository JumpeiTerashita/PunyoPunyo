#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 4
#define BUTTON_Y 8
#define BUTTON_L 16
#define BUTTON_R 32
#define BUTTON_SELECT 64
#define BUTTON_START 128

#include <time.h>
#include <stdlib.h>
#include "glut.h"
#include "Puyo.h"
#include "GameManager.h"
#include "InputManager.h"

int beforeAnalogX = 0, beforeAnalogY = 0;

Puyo::Puyo(SEQUENCE _sequence)
{
	sequence = _sequence;
}

Puyo::Puyo(int _x, int _y)
{
	FallTimer = 0;
	pos.set(_x, _y);
	ColorNumber = COLOR_RED;
	//ColorNumber = (COLORPATTERN)((rand() % 5)+1);
	ColorSetup(COLOR_RED);
	/*GameManager::getInstance()->addObject(this);*/
}

void Puyo::ObjUpdate()
{

	FallTimer++;
	if (input.DownKey == BUTTON_B)
	{
		if (pos._x < 5)
		{
			pos._x++;
		}
	}
	if (input.DownKey == BUTTON_X)
	{
		if (pos._x > 0)
		{
			pos._x--;
		}
	}
	if (input.AnalogY > 500)
	{
		FallTimer += 4;
	}


	if (FallTimer >= 60)
	{
		pos._y--;
		FallTimer = 0;
	}

	beforeAnalogX = input.AnalogX;
	beforeAnalogY = input.AnalogY;

	if (pos._y <= 0) sequence = STATE_TOUCHDOWN;


	//	pos._y--;
}


void Puyo::ObjDisp()
{
	glPushMatrix();
	{
		glColor4f(colorStatus[0], colorStatus[1], colorStatus[2], colorStatus[3]);
		glTranslatef(-0.7 + pos._x*0.15, -0.75 + pos._y*0.15, 1);
		glutSolidSphere(0.075, 180, 5);
	}
	glPopMatrix();

}

void Puyo::ColorSetup(COLORPATTERN _colorNumber)
{
	switch (_colorNumber)
	{
	case COLOR_RED:
		colorStatus[0] = 1;
		colorStatus[1] = 0;
		colorStatus[2] = 0;
		colorStatus[3] = 1;
		break;
	case COLOR_BLUE:
		colorStatus[0] = 0;
		colorStatus[1] = 0;
		colorStatus[2] = 1;
		colorStatus[3] = 1;
		break;
	case COLOR_GREEN:
		colorStatus[0] = 0;
		colorStatus[1] = 1;
		colorStatus[2] = 0;
		colorStatus[3] = 1;
		break;
	case COLOR_YELLOW:
		colorStatus[0] = 1;
		colorStatus[1] = 1;
		colorStatus[2] = 0;
		colorStatus[3] = 1;
		break;
	case COLOR_PURPLE:
		colorStatus[0] = 1;
		colorStatus[1] = 0;
		colorStatus[2] = 1;
		colorStatus[3] = 1;
	default:
		break;
	}
	return;
}