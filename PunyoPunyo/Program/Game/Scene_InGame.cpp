#define _CRT_SECURE_NO_WARNINGS
#include "Scene_InGame.h"
#include "../Engine/InputManager.h"
#include "../glut.h"
#include "BitCalc.h"
#include <string.h>

static unsigned const char KeyFlag_LEFT = 1 << KEY_LEFT;
static unsigned const char KeyFlag_RIGHT = 1 << KEY_RIGHT;
static unsigned const char KeyFlag_Turn_CounterClockwise = 1 << KEY_TURN_COUNTERCLOCKWISE;
static unsigned const char KeyFlag_Turn_Clockwise = 1 << KEY_TURN_CLOCKWISE;

unsigned const char Is_checked = 1 << IS_CHECKED;
unsigned const char Is_rotate = 1 << IS_ROTATE;
unsigned const char Will_Delete = 1 << WILL_DELETE;

SceneIngame::SceneIngame()
{

	cc = 0;

	First = nullptr;
	Second = nullptr;
	pFile = fopen(
		"Brick.data", // const char * _Filename
		"rb");      // const char * _Mode

	fread(
		pixels,        // void * _DstBuf
		300 * 300 * 3, // size_t _ElementSize
		1,          // size_t _Count
		pFile);     // FILE * _File


	fclose(pFile);// FILE * _File

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			map[i][j] = nullptr;
		}
	}
	PuyoCreate();
	CreatedMap();
	PuyoCreate();
	FallLimit = 30;

	PuyoCounter = 0;
	ScoreCounter = 0;
	ChainCounter = 0;
	AllVanishedNum = 0;
	HighScore = GameManager::getInstance()->HighScore;
	_is_vertical = true;
	GameOver = false;
	setSequence(&SceneIngame::Playing);
}

SceneIngame::~SceneIngame()
{
}

SceneIngame* SceneIngame::getInstance() {
	if (nullptr == SceneIngame::instance) {
		SceneIngame::instance = new SceneIngame();
	}
	return SceneIngame::instance;
}

void DrawString(const char* str)
{
	while (*str != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str++);
	}
}

void SceneIngame::PuyoCreate()
{
	First = new Puyo(3, 12, 4);
	//First->Status |= Is_rotate;
	Second = new Puyo(3, 13, 4);
	Second->_is_rotate = true;
}

void SceneIngame::CreatedMap()
{
	if (map[12][3] != nullptr)
	{
		GameOver = true;
		return;
	}
	GameManager::getInstance()->addObject(First);
	GameManager::getInstance()->addObject(Second);
	map[First->pos._y][First->pos._x] = First;
	map[Second->pos._y][Second->pos._x] = Second;
	_is_vertical = true;

	return;
}

void SceneIngame::CheckPuyo(int _x, int _y)
{
	SearchColor = map[_y][_x]->ColorNumber;

	//右
	if (_x + 1 < 7 && map[_y][_x + 1] != nullptr)
	{

		//if ((int)(map[_y][_x + 1]->Status&Is_checked)==0)
		if (map[_y][_x + 1]->_is_checked == false)
		{
			//map[_y][_x + 1]->Status |= Is_checked;
			map[_y][_x + 1]->_is_checked = true;
			if (map[_y][_x + 1]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x + 1, _y);
			}
		}
	}

	//上
	if (_y + 1 < 15 && map[_y + 1][_x] != nullptr)
	{
		//if ((int)(map[_y+1][_x]->Status&Is_checked)==0)
		if (map[_y + 1][_x]->_is_checked == false)
		{
			//map[_y+1][_x]->Status |= Is_checked;
			map[_y + 1][_x]->_is_checked = true;
			if (map[_y + 1][_x]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x, _y + 1);
			}
		}
	}

	//左
	if (_x - 1 > 0 && map[_y][_x - 1] != nullptr)
	{
		//if ((int)(map[_y][_x-1]->Status&Is_checked)==0)
		if (map[_y][_x - 1]->_is_checked == false)
		{
			//map[_y][_x-1]->Status |= Is_checked;
			map[_y][_x - 1]->_is_checked = true;
			if (map[_y][_x - 1]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x - 1, _y);
			}
		}
	}

	//下
	if (_y > 0 && map[_y - 1][_x] != nullptr)
	{
		//if ((int)(map[_y-1][_x]->Status&Is_checked)==0)
		if (map[_y - 1][_x]->_is_checked == false)
		{
			//map[_y-1][_x]->Status |= Is_checked;
			map[_y - 1][_x]->_is_checked = true;
			if (map[_y - 1][_x]->ColorNumber == SearchColor)
			{
				PuyoCounter++;
				CheckPuyo(_x, _y - 1);
			}
		}
	}
	return;
}

void SceneIngame::DeleteStart()
{
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (map[i][j] == nullptr) continue;
			PuyoCounter = 0;
			CheckPuyo(j, i);
			DeleteMarkSet();
		}
	}
	VanishPuyo();
	//if (VanishCounter == 0) 
	if (!DeletePuyoStatus) setSequence(&SceneIngame::FinishedVanish);
	else
	{
		DelScoreCalc();
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
				if (map[i][j] != nullptr)
				{
					//if ((int)(map[i][j]->Status&Is_checked)&&map[i][j]->ColorNumber==SearchColor)
					if (map[i][j]->_is_checked == true && map[i][j]->ColorNumber == SearchColor)
					{
						//map[i][j]->Status |= Will_Delete;
						map[i][j]->_will_delete = true;
					}
				}
			}
		}
	}

	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (map[i][j] == nullptr) continue;
			//map[i][j]->Status &= ~Is_checked;
			map[i][j]->_is_checked = false;
		}
	}
}



void SceneIngame::VanishPuyo()
{
	DeletePuyoStatus = 0;

	VanishCounter = 0;
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (map[i][j] != nullptr)
			{
				//if ((int)(map[i][j]->Status&Will_Delete))
				if (map[i][j]->_will_delete == true)
				{
					COLORPATTERN DelColor = map[i][j]->ColorNumber;
					unsigned int AddStatus = BitColPicker(DelColor, DeletePuyoStatus);
					AddStatus=1;
					unsigned int ResultStatus = AddBit(DelColor, DeletePuyoStatus, AddStatus);
					DeletePuyoStatus = ResultStatus;
					//TODO VanishCounter Colorごとのカウントも！
					//VanishCounter++;
					map[i][j] = nullptr;
				}
				else
				{
					COLORPATTERN RefallColor = map[i][j]->ColorNumber;
					map[i][j] = nullptr;
					Puyo* Refall = new Puyo(j, i, RefallColor);
					Refall->_is_falling = true;
					Refall->_is_freefall = true;
					GameManager::getInstance()->addObject(Refall);
					Refall->setSequence(&Puyo::FreeFall);
					map[i][j] = Refall;
				}
			}
		}
	}

}

void SceneIngame::WaitingRestart()
{

	if (getFrame() >= 60 && _Key)
	{
		GameManager::getInstance()->HighScore = HighScore;
		instance = nullptr;
		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報）
		GameManager::getInstance()->setSequence(&GameManager::Scene_Title);
	}

}

void SceneIngame::KeyJudge()
{

	cc = 0;

	GameManager* Game = GameManager::getInstance();
	if (_SpecialKey == GLUT_KEY_RIGHT || _Key == 'd')
	{

		cc = 1;

		if (_is_vertical)
		{
			if (Game->MoveSearch(1) == true)
			{
				KeyFlag |= KeyFlag_RIGHT;
			}
			else return;
		}
		else
		{
			KeyFlag |= KeyFlag_RIGHT;
		}
	}

	if (_SpecialKey == GLUT_KEY_LEFT || _Key == 'a')
	{
		cc = 1;

		if (_is_vertical)
		{
			if (Game->MoveSearch(-1) == true)
			{
				KeyFlag |= KeyFlag_LEFT;
			}
			else return;
		}
		else
		{
			KeyFlag |= KeyFlag_LEFT;
		}
	}
	if (_UpKey == 'j')
	{
		cc = 1;
		KeyFlag |= KeyFlag_Turn_CounterClockwise;
	}
	if (_UpKey == 'k')
	{
		cc = 1;
		KeyFlag |= KeyFlag_Turn_Clockwise;
	}
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
	if (GameManager::getInstance()->getObject().size() == 0)
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
	if (GameManager::getInstance()->getObject().size() == 1)
	{

		Puyo* puyoit = dynamic_cast<Puyo*>(*GameManager::getInstance()->getObjectsIterator());
		puyoit->_is_freefall = true;
	}
	runSequence();
}

void SceneIngame::DelScoreCalc()
{
	//TODO 同時消しボーナス追加


	const unsigned int VanishedPuyo = DeletePuyoStatus;
	const int ChainNum = ChainCounter;
	const int ChainBonusBox[20] = { 0,8,16,32,64,96,128,160,192,224,256,288,320,352,388,416,448,480,512,512 };
	int PlusScore = 0;
	int ChainBonus = 0;
	int ColorResultStatus[4] = { 0 };
	int VanishedNum = 0;
	int VanishNumBonus = 0;
	int VanishColorsNum = 0;
	int VanishColorsBonus = 0;
	const int VanishColorsBonusBox[5] = { 0,0,3,6,12 };

	if (ChainNum >= 19) ChainBonus = ChainBonusBox[19];
	else ChainBonus = ChainBonusBox[ChainNum];

	for (int i = 0; i < 4; i++)
	{
		ColorResultStatus[i] = BitColPicker((COLORPATTERN)i, VanishedPuyo);
		if (ColorResultStatus[i])
		{
			VanishedNum += ColorResultStatus[i];
			AllVanishedNum += ColorResultStatus[i]; 
			VanishColorsNum++;
		}
		if (ColorResultStatus[i] <= 4) continue;
		if (ColorResultStatus[i] == 5) VanishNumBonus += 2;
		if (ColorResultStatus[i] == 6) VanishNumBonus += 3;
		if (ColorResultStatus[i] == 7) VanishNumBonus += 4;
		if (ColorResultStatus[i] == 8) VanishNumBonus += 5;
		if (ColorResultStatus[i] == 9) VanishNumBonus += 6;
		if (ColorResultStatus[i] == 10) VanishNumBonus += 7;
		if (ColorResultStatus[i] >= 11) VanishNumBonus += 10;
	}

	VanishColorsBonus = VanishColorsBonusBox[VanishColorsNum];

	int AllBonus = ChainBonus + VanishNumBonus + VanishColorsBonus;
	if (AllBonus == 0) AllBonus = 1;

	PlusScore = VanishedNum * 10 * AllBonus;

	ScoreCounter += PlusScore;


	/*int PlusScore = 0;
	const int VanishedPuyo = VanishCounter;
	const int ChainNum = ChainCounter;
	const int ChainBonusBox[20] = { 0,8,16,32,64,96,128,160,192,224,256,288,320,352,388,416,448,480,512,512 };
	int ChainBonus = 0;
	if (ChainNum >= 19) ChainBonus = ChainBonusBox[19];
	else ChainBonus = ChainBonusBox[ChainNum];

	int VanishNumBonus = 0;
	if (VanishedPuyo == 5) VanishNumBonus = 2;
	if (VanishedPuyo == 6) VanishNumBonus = 3;
	if (VanishedPuyo == 7) VanishNumBonus = 4;
	if (VanishedPuyo == 8) VanishNumBonus = 5;
	if (VanishedPuyo == 9) VanishNumBonus = 6;
	if (VanishedPuyo == 10) VanishNumBonus = 7;
	if (VanishedPuyo >= 11) VanishNumBonus = 10;

	int AllBonus = ChainBonus + VanishNumBonus;
	if (AllBonus == 0) AllBonus = 1;

	PlusScore = VanishedPuyo * 10 * AllBonus;

	ScoreCounter += PlusScore;
	AllVanishedNum += VanishedPuyo;*/
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
		pixels);            // const GLvoid *pixels

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

}

void SceneIngame::display()
{
	UIDisp();
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (map[i][j] == nullptr) continue;
			glPushMatrix();
			{
				glColor4f(map[i][j]->colorStatus[0], map[i][j]->colorStatus[1], map[i][j]->colorStatus[2], map[i][j]->colorStatus[3]);
				glTranslatef(-0.7 + (j - 1)*0.15, -0.75 + (i - 1)*0.15, 1);
				glutSolidSphere(0.075, 180, 5);
			}
			glPopMatrix();
		}
	}

	return;
}

