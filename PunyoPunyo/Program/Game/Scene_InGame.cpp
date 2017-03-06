#define _CRT_SECURE_NO_WARNINGS
#include "Scene_InGame.h"
#include "../Engine/InputManager.h"
#include "../glut.h"
#include "../Liblary/BitController.h"
#include "../Engine/DrawString.h"
#include <string.h>

static const unsigned char KeyFlag_LEFT = 1 << KEY_LEFT;
static const unsigned char KeyFlag_RIGHT = 1 << KEY_RIGHT;
static const unsigned char KeyFlag_Turn_CounterClockwise = 1 << KEY_TURN_COUNTERCLOCKWISE;
static const unsigned char KeyFlag_Turn_Clockwise = 1 << KEY_TURN_CLOCKWISE;

SceneIngame::SceneIngame()
{
	First = nullptr;
	Second = nullptr;


	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Map[i][j] = nullptr;
		}
	}
	DeleteCounterReset();
	PuyoCreate();
	CreatedMap();
	PuyoCreate();
	FallLimit = 30;
	ArrangeRelation = ARRANGERELATION_VERTICAL;
	PuyoCounter = 0;
	ScoreCounter = 0;
	ChainCounter = 0;
	AllVanishedNum = 0;
	HighScore = GameManager::GetInstance()->HighScore;
	GameOver = false;
	setSequence(&SceneIngame::Playing);
}

SceneIngame::~SceneIngame()
{
}

SceneIngame* SceneIngame::GetInstance() {
	if (nullptr == SceneIngame::Instance) {
		SceneIngame::Instance = new SceneIngame();
	}
	return SceneIngame::Instance;
}

void SceneIngame::PuyoCreate()
{
	First = new Puyo(3, 12, Difficulty + 3);
	Second = new Puyo(3, 13, Difficulty + 3);

	BitAddition(&First->Status, Is_rotate);
}

void SceneIngame::CreatedMap()
{
	ArrangeRelation = ARRANGERELATION_VERTICAL;
	if (Map[12][3] != nullptr)
	{
		GameOver = true;
		return;
	}
	for (int i = 1; i < 7; i++)
	{
		if (Map[13][i] != nullptr)
		{
			GameOver = true;
			return;
		}
	}
	GameManager::GetInstance()->addObject(First);
	GameManager::GetInstance()->addObject(Second);
	Map[First->pos._y][First->pos._x] = First;
	Map[Second->pos._y][Second->pos._x] = Second;
	return;
}

void SceneIngame::WriteMap(int _x, int _y, COLORPATTERN _SetColor)
{
	if (_SetColor == COLOR_NULL) Map[_y][_x] = nullptr;
	else Map[_y][_x] = new Puyo(_x, _y, _SetColor);
}

void SceneIngame::CheckPuyo(int _x, int _y)
{
	SearchColor = Map[_y][_x]->ColorNumber;

	//右
	if (_x + 1 < 7 && Map[_y][_x + 1] != nullptr)
	{
		if (!BitChecker(Map[_y][_x + 1]->Status, Is_checked))
		{
			BitAddition(&Map[_y][_x + 1]->Status, Is_checked);
			if (Map[_y][_x + 1]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x + 1, _y);
			}
		}
	}

	//上
	if (_y + 1 < 15 && Map[_y + 1][_x] != nullptr)
	{
		if (!BitChecker(Map[_y + 1][_x]->Status, Is_checked))
		{
			BitAddition(&Map[_y + 1][_x]->Status, Is_checked);
			if (Map[_y + 1][_x]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x, _y + 1);
			}
		}
	}

	//左
	if (_x - 1 > 0 && Map[_y][_x - 1] != nullptr)
	{
		if (!BitChecker(Map[_y][_x - 1]->Status, Is_checked))
		{
			BitAddition(&Map[_y][_x - 1]->Status, Is_checked);
			if (Map[_y][_x - 1]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x - 1, _y);
			}
		}
	}

	//下
	if (_y > 0 && Map[_y - 1][_x] != nullptr)
	{
		if (!BitChecker(Map[_y - 1][_x]->Status, Is_checked))
		{
			BitAddition(&Map[_y - 1][_x]->Status, Is_checked);
			if (Map[_y - 1][_x]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x, _y - 1);
			}
		}
	}
	return;
}

void SceneIngame::DeleteCounterAdd(COLORPATTERN _DeleteColor)
{
	DeleteCounter[_DeleteColor]++;
}

void SceneIngame::DeleteCounterReset()
{
	for (int i = 0; i < 7; i++)
	{
		DeleteCounter[i] = 0;
	}
}

int SceneIngame::DeleteCounterSum()
{
	int AllDeletePuyos = 0;
	for (int i = 0; i < 7; i++)
	{
		AllDeletePuyos += DeleteCounter[i];
	}
	return AllDeletePuyos;
}

void SceneIngame::DeleteStart()
{
	ArrangeRelation = ARRANGERELATION_REFALL;
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (Map[i][j] == nullptr) continue;
			PuyoCounter = 0;
			CheckPuyo(j, i);
			DeleteMarkSet();
		}
	}
	VanishPuyo();

	if (DeleteCounterSum() == 0) setSequence(&SceneIngame::FinishedVanish);
	else
	{
		DelScoreCalc();
		DeleteCounterReset();
		ChainCounter++;
		setSequence(&SceneIngame::Playing);
	}
}

void SceneIngame::DeleteMarkSet()
{
	if (PuyoCounter >= 4)
	{
		for (int i = 1; i < 15; i++)
		{
			for (int j = 1; j < 7; j++)
			{
				if (Map[i][j] != nullptr)
				{
					if (BitChecker(Map[i][j]->Status, Is_checked) && Map[i][j]->ColorNumber == SearchColor)
					{
						BitAddition(&Map[i][j]->Status, Will_Delete);
					}
				}
			}
		}
	}

	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (Map[i][j] == nullptr) continue;
			BitTakeaway(&Map[i][j]->Status, Is_checked);
		}
	}
}



void SceneIngame::VanishPuyo()
{

	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (Map[i][j] != nullptr)
			{
				if (BitChecker(Map[i][j]->Status, Will_Delete))
				{
					COLORPATTERN DelColor = Map[i][j]->ColorNumber;
					DeleteCounterAdd(DelColor);
					Map[i][j] = nullptr;
				}
				else
				{
					COLORPATTERN RefallColor = Map[i][j]->ColorNumber;
					Map[i][j] = nullptr;
					Puyo* Refall = new Puyo(j, i, RefallColor);
					BitAddition(&Refall->Status, Is_falling | Is_Freefall);
					GameManager::GetInstance()->addObject(Refall);
					Refall->setSequence(&Puyo::FreeFall);
					Map[i][j] = Refall;
				}
			}
		}
	}

}

void SceneIngame::WaitingRestart()
{

	if (getFrame() >= 60 && _Key)
	{
		GameManager::GetInstance()->HighScore = HighScore;
		Instance = nullptr;
		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報）
		GameManager::GetInstance()->setSequence(&GameManager::Scene_Title);
	}

}

void SceneIngame::KeyJudge()
{
	GameManager* Game = GameManager::GetInstance();
	if (_SpecialKey == GLUT_KEY_RIGHT || _Key == 'd')
	{

		if (ArrangeRelation == ARRANGERELATION_VERTICAL || ArrangeRelation == ARRANGERELATION_COUNTERVERTICAL)
		{
			if (Game->MoveSearch(1) == true)
			{
				BitAddition(&KeyFlag, KeyFlag_RIGHT);
			}
			else return;
		}
		else
		{
			BitAddition(&KeyFlag, KeyFlag_RIGHT);
		}
	}

	if (_SpecialKey == GLUT_KEY_LEFT || _Key == 'a')
	{

		if (ArrangeRelation == ARRANGERELATION_VERTICAL || ArrangeRelation == ARRANGERELATION_COUNTERVERTICAL)
		{
			if (Game->MoveSearch(-1) == true)
			{
				BitAddition(&KeyFlag, KeyFlag_LEFT);
			}
			else return;
		}
		else
		{
			BitAddition(&KeyFlag, KeyFlag_LEFT);
		}
	}
	if (_UpKey == 'j')	BitAddition(&KeyFlag, KeyFlag_Turn_CounterClockwise);
	
	if (_UpKey == 'k')	BitAddition(&KeyFlag, KeyFlag_Turn_Clockwise);

	if (_SpecialKey == GLUT_KEY_DOWN || _Key == 's')
	{
		ScoreCounter++;
		FallLimit = 0;
	}
	else FallLimit = 30;
}

void SceneIngame::Playing()
{
	KeyFlag = 0;

	KeyJudge();

	if (GameOver)
	{
		WaitingRestart();
		setSequence(&SceneIngame::WaitingRestart);
	}
	if (GameManager::GetInstance()->getObject().size() == 0)
	{
		setSequence(&SceneIngame::DeleteStart);
	}
}

void SceneIngame::FinishedVanish()
{
	ChainCounter = 0;
	CreatedMap();
	PuyoCreate();
	setSequence(&SceneIngame::Playing);
}

void SceneIngame::update()
{
	if (GameManager::GetInstance()->getObject().size() == 1)
	{
		Puyo* puyoit = dynamic_cast<Puyo*>(*GameManager::GetInstance()->getObjectsIterator());
		if (puyoit->getSequence() == &Puyo::Fall)
		{
			puyoit->setSequence(&Puyo::FreeFall);
		}
	}
	runSequence();
}

void SceneIngame::DelScoreCalc()
{
	const int VanishedPuyo = DeleteCounterSum();
	const int ChainNum = ChainCounter;
	const int ChainBonusBox[20] = { 0,8,16,32,64,96,128,160,192,224,256,288,320,352,388,416,448,480,512,512 };
	int PlusScore = 0;
	int ChainBonus = 0;
	int VanishedNum = 0;
	int VanishNumBonus = 0;
	const int VanishNumBonusBox[12] = { 0,0,0,0,0,2,3,4,5,6,7,10 };
	int VanishColorsNum = 0;
	int VanishColorsBonus = 0;
	const int VanishColorsBonusBox[7] = { 0,0,3,6,12,2400,48000 };

	if (ChainNum >= 19) ChainBonus = ChainBonusBox[19];
	else ChainBonus = ChainBonusBox[ChainNum];

	for (int i = 0; i < 7; i++)
	{
		if (DeleteCounter[i])
		{
			VanishedNum += DeleteCounter[i];
			AllVanishedNum += DeleteCounter[i];
			VanishColorsNum++;
		}
		if (DeleteCounter[i] >= 11) VanishNumBonus += VanishNumBonusBox[11];
		else VanishNumBonus += VanishNumBonusBox[DeleteCounter[i]];
	}

	VanishColorsBonus = VanishColorsBonusBox[VanishColorsNum];

	int AllBonus = ChainBonus + VanishNumBonus + VanishColorsBonus;
	if (AllBonus == 0) AllBonus = 1;

	PlusScore = VanishedNum * 10 * AllBonus;

	ScoreCounter += PlusScore;
}

void SceneIngame::UIDisp()
{
	glColor4f(1, 1, 1, 1);
	//テクスチャ有効化
	glEnable(GL_TEXTURE_2D);

	//テクスチャ生成
	glTexImage2D(
		GL_TEXTURE_2D,      // テクスチャ種類 (GLenum target)
		0,                  // ミップマップレベル (GLint level)
		GL_RGB,             // テクスチャフォーマット (GLint internalformat)
		300, 300,               // 幅と高さ (GLsizei width, height)
		0,                  // ボーダー処理 (GLint border)
		GL_RGB,             // ピクセルフォーマット (GLenum format)
		GL_UNSIGNED_BYTE,   // 各ピクセルのデータ型 (GLenum type)
		GameManager::GetInstance()->BrickPixels);            // const GLvoid *BrickPixels

							//テクスチャフィルター設定 (テクスチャ描画時必須)
							//_________________________________________________
							//ピクセル縮小時フィルター
	glTexParameteri(
		GL_TEXTURE_2D,          // GLenum target
		GL_TEXTURE_MIN_FILTER,  // GLenum pname
		GL_NEAREST);            // GLint param
								//ピクセル拡大時フィルター_________________________
	glTexParameteri(
		GL_TEXTURE_2D,          // GLenum target
		GL_TEXTURE_MAG_FILTER,  // GLenum pname
		GL_NEAREST);            // GLint param
								//_________________________________________________

								//テクスチャ座標 頂点配列
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	float TexCoords[] = {
		0,5,
		5,5,
		0,0,
		5,0
	};
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);

	//頂点配列
	float vertices[] = {
		-1,-1,
		1,-1,
		-1,1,
		1,1
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBegin(GL_TRIANGLE_STRIP);	// マップデータ　ブロック
	glColor4f(0, 0, 0, 1);
	glVertex2f(-0.78, -0.825);
	glVertex2f(-0.78, 1);
	glVertex2f(0.13, -0.825);
	glVertex2f(-0.78, 1);
	glVertex2f(0.13, -0.825);
	glVertex2f(0.13, 1);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);	// マップデータ　ブロック
	glColor4f(0, 0, 0, 1);
	glVertex2f(0.25, 0.8);
	glVertex2f(0.25, 0.2);
	glVertex2f(0.9, 0.2);
	glVertex2f(0.9, 0.8);
	glVertex2f(0.25, 0.8);
	glVertex2f(0.9, 0.2);
	glEnd();

	glPushMatrix();
	{
		glColor4f(Second->colorStatus[0], Second->colorStatus[1], Second->colorStatus[2], Second->colorStatus[3]);
		glTranslatef(0.575f, 0.55f, 1);
		glutSolidSphere(0.075, 180, 5);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor4f(First->colorStatus[0], First->colorStatus[1], First->colorStatus[2], First->colorStatus[3]);
		glTranslatef(0.575f, 0.4f, 1);
		glutSolidSphere(0.075, 180, 5);

	}
	glPopMatrix();

	glBegin(GL_TRIANGLE_STRIP);	// マップデータ　ブロック
	glColor4f(0, 0, 0, 1);
	glVertex2f(0.25, 0.1);
	glVertex2f(0.25, -0.3);
	glVertex2f(0.9, -0.3);
	glVertex2f(0.9, 0.1);
	glVertex2f(0.25, 0.1);
	glVertex2f(0.9, -0.3);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);	// マップデータ　ブロック
	glColor4f(0, 0, 0, 1);
	glVertex2f(0.25, -0.4);
	glVertex2f(0.25, -0.825);
	glVertex2f(0.9, -0.825);
	glVertex2f(0.9, -0.4);
	glVertex2f(0.25, -0.4);
	glVertex2f(0.9, -0.825);
	glEnd();

	char Score[256];
	sprintf(Score, "%08d", ScoreCounter);
	char Block[256];
	sprintf(Block, "%8d", AllVanishedNum);
	if (ScoreCounter >= HighScore)
	{
		HighScore = ScoreCounter;
	}
	char Score_High[256];
	sprintf(Score_High, "%08d", HighScore);
	glPushMatrix();
	{
		glColor4f(1, 0.5f, 0.5f, 1);
		glRasterPos2f(0.3f, 0.7f);
		DrawString("NEXT");
		glRasterPos2f(0.3f, 0);
		DrawString("SCORE");
		glRasterPos2f(0.6f, 0);
		DrawString(Score);
		glRasterPos2f(0.3f, -0.175f);
		DrawString("HIGH");
		glRasterPos2f(0.35f, -0.225f);
		DrawString("SCORE");
		glRasterPos2f(0.6f, -0.2f);
		DrawString(Score_High);
		glRasterPos2f(0.3f, -0.5f);
		DrawString("BLOCK");
		glRasterPos2f(0.68f, -0.6f);
		DrawString(Block);
		glColor4f(0, 0, 0, 1);
		glRasterPos2f(-0.8f, -0.95f);
		DrawString("    MOVE =  A or D      KEY  FALL = S    SPIN = J or K ");
	}
	glPopMatrix();

	if (GameOver)
	{
		glPushMatrix();
		{
			glColor4f(1, 0, 0, 1);
			glRasterPos2f(0.4f, -0.7f);
			DrawString("GAME OVER");
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor4f(1, 1, 1, 1);
			glRasterPos2f(0.36f, -0.775f);
			DrawString("PUSH ANY KEY");
		}
		glPopMatrix();
	}

	if (ArrangeRelation == ARRANGERELATION_REFALL && ChainCounter > 0)
	{
		char Chain[2];
		sprintf(Chain, "%d", ChainCounter);
		glPushMatrix();
		{
			glColor4f(1, 0, 0, 1);
			glRasterPos2f(0.4f, -0.7f);
			DrawString(Chain);
			glRasterPos2f(0.5f, -0.7f);
			DrawString("CHAIN !");
		}
		glPopMatrix();
	}
}

void SceneIngame::display()
{
	UIDisp();
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (Map[i][j] == nullptr) continue;
			glPushMatrix();
			{
				glColor4f(Map[i][j]->colorStatus[0], Map[i][j]->colorStatus[1], Map[i][j]->colorStatus[2], Map[i][j]->colorStatus[3]);
				glTranslatef(-0.7 + (j - 1)*0.15, -0.75 + (i - 1)*0.15, 1);
				glutSolidSphere(0.075, 180, 5);
			}
			glPopMatrix();
		}
	}

	return;
}

