#pragma once

#include "GameManager.h"
#include "Puyo.h"
#include "GameObject.h"

class SceneIngame : public Sequence<SceneIngame>
{
public:
	SceneIngame();
	~SceneIngame();

	static SceneIngame* getInstance();

	int PuyoColorsNum;
	bool GameOver;
	Puyo* map[15][8];
	float ScoreCounter;
	int FallLimit;
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
	bool Keyflag_left;
	bool Keyflag_right;
	bool Keyflag_turnCounterClockwise;
	bool Keyflag_turnClockwise;
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