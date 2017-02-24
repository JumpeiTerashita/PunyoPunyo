#pragma once

typedef struct {
	unsigned char LastKey;
	unsigned char PressedKey;
	unsigned char ReleasedKey;
	unsigned char ChangedKey;
	unsigned char DownKey;
	unsigned char UpKey;
	int AnalogX;
	int AnalogY;
}INPUTMANAGER;

extern unsigned char _UpKey;
extern unsigned char _Key;
extern int _SpecialKey;
extern INPUTMANAGER Controller;
extern void JoyStick(unsigned int buttonMask, int x, int y, int z);
extern void UpKey(unsigned char _key, int _x, int _y);
extern void Key(unsigned char _key, int _x, int _y);
extern void SpecialKey(int key, int x, int y);
extern void KeyEventReset();

