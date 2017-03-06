#pragma once

#include "GameManager.h"
#include "Puyo.h"
#include "../Engine/GameObject.h"

enum TURNPATTERN
{
	ARRANGERELATION_VERTICAL,
	ARRANGERELATION_RIGHT,
	ARRANGERELATION_COUNTERVERTICAL,
	ARRANGERELATION_LEFT,
	ARRANGERELATION_REFALL
};

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

	static SceneIngame* GetInstance();

	void update();
	void display();

	int DeleteCounter[7]; 
	
	Puyo* Map[15][8];
	int ScoreCounter;
	int FallLimit;
	int HighScore;
	bool GameOver;
	void PuyoCreate();
	void CreatedMap();
	void WriteMap(int _x, int _y, COLORPATTERN _SetColor);
	void Playing();
	void KeyJudge();
	unsigned char KeyFlag;
	TURNPATTERN ArrangeRelation;

	inline void SetDifficulty(int _Difficulty) { Difficulty = _Difficulty; }

private:
	int Difficulty;
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

	void DeleteCounterReset();
	void DeleteCounterAdd(COLORPATTERN _DeleteColor);
	int DeleteCounterSum();

	void UIDisp();
	void DelScoreCalc();

	static SceneIngame* Instance;
};

extern unsigned char _Key;
extern const unsigned char KeyFlag_LEFT,KeyFlag_RIGHT ,KeyFlag_Turn_CounterClockwise,KeyFlag_Turn_Clockwise;
extern const unsigned char Is_checked ,Is_rotate,Is_falling,Is_Freefall,Will_Delete;
