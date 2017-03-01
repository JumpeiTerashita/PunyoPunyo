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

	static SceneIngame* getInstance();

	int cc; //デバッグ用　ぷよ処理通過カウンタ

	unsigned int DeletePuyoStatus; //(赤,緑,青,黄色)*8ビット
	
	Puyo* map[15][8];
	int ScoreCounter;
	int FallLimit;
	bool GameOver;

	void PuyoCreate();
	void CreatedMap();
	void update();
	void display();
	void Playing();
	void KeyJudge();

	int HighScore;

	
	unsigned char KeyFlag;

	TURNPATTERN ArrangeRelation;

private:
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
extern const unsigned char KeyFlag_LEFT,KeyFlag_RIGHT ,KeyFlag_Turn_CounterClockwise,KeyFlag_Turn_Clockwise;
extern const unsigned char Is_checked ,Is_rotate,Is_falling,Is_Freefall,Will_Delete;
