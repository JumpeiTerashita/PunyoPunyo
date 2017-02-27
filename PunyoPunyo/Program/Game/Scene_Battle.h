#pragma once

#include "GameManager.h"
#include "Puyo.h"
#include "../Engine/GameObject.h"

enum KEYFLAGS
{
	KEY_LEFT,
	KEY_RIGHT,
	KEY_TURN_COUNTERCLOCKWISE,
	KEY_TURN_CLOCKWISE
};



class SceneBattle : public Sequence<SceneBattle>
{
public:
	SceneBattle();
	~SceneBattle();

	static SceneBattle* getInstance();

	int cc;

	unsigned int DeletePuyoStatus; //(赤,緑,青,黄色)*8ビット

	BattlePuyo* map[15][8];
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



	int HighScore;


	unsigned char KeyFlag;

private:

	int VanishCounter;
	int AllVanishedNum;
	int ChainCounter;
	int PuyoCounter;
	COLORPATTERN_BATTLE SearchColor;
	void CheckPuyo(int _x, int _y);
	void DeleteMarkSet();
	void VanishPuyo();
	void FinishedVanish();
	void DeleteStart();
	void WaitingRestart();

	BattlePuyo* First;
	BattlePuyo* Second;

	unsigned char* pixels = (unsigned char*)malloc(300 * 300 * 3);
	FILE *pFile;

	void UIDisp();
	void DelScoreCalc();

	static SceneBattle* instance;
};

extern unsigned char _Key;
extern unsigned const char KeyFlag_LEFT, KeyFlag_RIGHT, KeyFlag_Turn_CounterClockwise, KeyFlag_Turn_Clockwise;
