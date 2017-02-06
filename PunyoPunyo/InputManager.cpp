#include <stdio.h>
#include <stdlib.h>
#include "InputManager.h"
#include "glut.h"

INPUTMANAGER Controller = { 0 };
unsigned char _UpKey;
unsigned char _Key;
int _SpecialKey;


void Key(unsigned char _key, int _x, int _y)
{
	_Key = _key;
	//glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報）
	
	//printf("keyboard key = %d x = %d y = %d\n", _Key, _x, _y);

}

void UpKey(unsigned char _key, int _x, int _y)
{
	_UpKey = _key;
	//glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報）

	//printf("keyboard key = %d x = %d y = %d\n", _Key, _x, _y);

}

void SpecialKey(int key, int x, int y)
{
	_SpecialKey = key;
	//glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報）

	//printf("keyboard key = %c x = %d y = %d\n", _SpecialKey, x, y);
}

void KeyEventReset()
{
	_Key = 0;
	_SpecialKey = 0;
}

void JoyStick(unsigned int buttonMask, int x, int y, int z)
{
	Controller.AnalogX += x - Controller.AnalogX;
	Controller.AnalogY += y - Controller.AnalogY;
	Controller.LastKey = Controller.PressedKey;
	//glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報）

	//printf("joystick buttonMask:%d x:%d y:%d z:%d\n", buttonMask, x, y, z);
	Controller.PressedKey = buttonMask;
	Controller.ReleasedKey = ~buttonMask;
	Controller.ChangedKey = (Controller.PressedKey ^ Controller.LastKey);
	Controller.UpKey = (Controller.ChangedKey & Controller.ReleasedKey);
	Controller.DownKey = (Controller.ChangedKey & Controller.PressedKey);
}