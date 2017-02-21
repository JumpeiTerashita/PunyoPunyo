#pragma once

#include "GameManager.h"
#include "Puyo.h"
#include "GameObject.h"

enum KEYFLAGS
{
	KEY_LEFT,
	KEY_RIGHT,
	KEY_TURN_COUNTERCLOCKWISE,
	KEY_TURN_CLOCKWISE
};

class SceneIngame : public Sequence<SceneIngame>
{
public:
	SceneIngame();
	~SceneIngame();

	static SceneIngame* getInstance();

	int PuyoColorsNum;
	
	Puyo* map[15][8];
	int ScoreCounter;
	int FallLimit;
	bool GameOver;
	bool _is_vertical;

	void PuyoCreate();
	void CreatedMap();
	void update();
	void display();
	void Playing();
	void KeyJudge();


	bool Search_State_is(STATE _state);
	int Search_States_are(STATE _state);
	int HighScore;

	unsigned char KeyFlag;
	unsigned const char KeyFlag_LEFT = 1 << KEY_LEFT;
	unsigned const char KeyFlag_RIGHT = 1 << KEY_RIGHT;
	unsigned const char KeyFlag_Turn_CounterClockwise = 1 << KEY_TURN_COUNTERCLOCKWISE;
	unsigned const char KeyFlag_Turn_Clockwise = 1 << KEY_TURN_CLOCKWISE;

private:

	int VanishCounter;
	int AllVanishedNum;
	int ChainCounter;
	int PuyoCounter;
	COLORPATTERN SearchColor;
	void CheckPuyo(int _x, int _y);
	void DeleteMarkSet();
	void VanishPuyo();
	void FinishedVanish();
	void DeleteStart();
	void WaitingRestart();
	Puyo* First;
	Puyo* Second;

	unsigned char* pixels = (unsigned char*)malloc(300 * 300 * 3);
	FILE *pFile;

	void UIDisp();
	void DelScoreCalc();

	static SceneIngame* instance;
};

extern unsigned char _Key;