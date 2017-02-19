#define _CRT_SECURE_NO_WARNINGS
#include "Scene_InGame.h"
#include "InputManager.h"
#include "glut.h"
#include <string.h>

SceneIngame::SceneIngame()
{
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
	turnCounter = 0;
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
	First = new Puyo(3, 10, 4);
	First->_is_rotate = true;
	Second = new Puyo(3, 11, 4);
	/*if (AllVanishedNum<20)
	{
		First = new Puyo(3, 10,3);
		Second = new Puyo(3, 11,3);
	}
	else if (AllVanishedNum < 50)
	{
		First = new Puyo(3, 10, 4);
		Second = new Puyo(3, 11, 4);
	}
	else if (AllVanishedNum < 150)
	{
		First = new Puyo(3, 10, 5);
		Second = new Puyo(3, 11, 5);
	}
	else
	{
		First = new Puyo(3, 10, 6);
		Second = new Puyo(3, 11, 6);
	}
	*/

}

void SceneIngame::CreatedMap()
{
	GameManager::getInstance()->addObject(First);
	GameManager::getInstance()->addObject(Second);
	map[First->pos._y][First->pos._x] = First;
	map[Second->pos._y][Second->pos._x] = Second;
	return;
}

void SceneIngame::CheckPuyo(int _x, int _y)
{
	SearchColor = map[_y][_x]->ColorNumber;

	//右
	if (_x + 1 < 7 && map[_y][_x + 1] != nullptr)
	{
		if (map[_y][_x + 1]->_is_checked == false)
		{
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
		if (map[_y + 1][_x]->_is_checked == false)
		{
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
		if (map[_y][_x - 1]->_is_checked == false)
		{
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
		if (map[_y - 1][_x]->_is_checked == false)
		{
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
//TODO 同時消し実装
void SceneIngame::DeleteStart()
{
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			if (map[i][j] == nullptr) continue;
			PuyoCounter = 0;
			CheckPuyo(j, i);
			VanishPuyo();
		}
	}

}


void SceneIngame::VanishPuyo()
{
	VanishCounter = 0;
	if (PuyoCounter >= 4)
	{
		for (int i = 1; i < 15; i++)
		{
			for (int j = 1; j < 8; j++)
			{

				if (map[i][j] != nullptr)
				{
					if (map[i][j]->_is_checked == true && map[i][j]->ColorNumber == SearchColor)
					{
						VanishCounter++;
						map[i][j] = nullptr;
					}
					else
					{
						COLORPATTERN RefallColor = map[i][j]->ColorNumber;
						map[i][j] = nullptr;
						Puyo* Refall = new Puyo(j, i, STATE_AFTERDELETE, RefallColor);
						GameManager::getInstance()->addObject(Refall);
						Refall->setSequence(&Puyo::FreeFall);
						map[i][j] = Refall;
					}
				}

			}
		}
		ChainCounter++;
		DelScoreCalc();
	}

	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			if (map[i][j] == nullptr) continue;

			map[i][j]->_is_checked = false;

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
	/*if (_Key)
	{
		PuyoColorsNum = 3;
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
		PuyoCounter = 0;
		ScoreCounter = 0;
		ChainCounter = 0;
		AllVanishedNum = 0;
		GameOver = false;
	}*/


}

void SceneIngame::KeyJudge()
{
	if (_SpecialKey == GLUT_KEY_RIGHT || _Key == 'd')
	{
		if (_is_vertical)
		{
			if (GameManager::getInstance()->MoveSearch(1))
			{
				Keyflag_right = true;
			}
		}
		else Keyflag_right = true;
	}
	
		//(input.AnalogX > 700 && getFrame() % 60 == 0)
	
	if (_SpecialKey == GLUT_KEY_LEFT || _Key == 'a')
	{
		if (_is_vertical)
		{
			if (GameManager::getInstance()->MoveSearch(-1))
			{
				Keyflag_left = true;
			}
		}
		else Keyflag_left = true;
	}
	if (_UpKey == 'j') Keyflag_turnCounterClockwise = true;
	if (_UpKey == 'k') Keyflag_turnClockwise = true;
	if (_SpecialKey == GLUT_KEY_DOWN || _Key == 's')
	{
		ScoreCounter += 1;
		FallLimit = 0;
	}
	else FallLimit = 30;
}

void SceneIngame::Playing()
{

	Keyflag_left = false;
	Keyflag_right = false;
	Keyflag_turnCounterClockwise = false;
	Keyflag_turnClockwise = false;
	KeyJudge();

	if (GameOver)
	{
		WaitingRestart();
		setSequence(&SceneIngame::WaitingRestart);
	}
	if (!Search_State_is(STATE_FALL))
	{
		turnCounter = 0;
		DeleteStart();
	}
	if (!Search_State_is(STATE_FALL) && !Search_State_is(STATE_AFTERDELETE))
	{
		ChainCounter = 0;
		CreatedMap();
		PuyoCreate();
	}


}

void SceneIngame::update()
{
	runSequence();

}

void SceneIngame::DelScoreCalc()
{
	int PlusScore = 0;
	const int VanishPuyo = VanishCounter;
	const int ChainNum = ChainCounter;
	const int ChainBonusBox[20] = { 0,0,8,16,32,64,96,128,160,192,224,256,288,320,352,388,416,448,480,512 };

	int ChainBonus = ChainBonusBox[ChainNum];

	int VanishNumBonus = 0;
	if (VanishPuyo == 5) VanishNumBonus = 2;
	if (VanishPuyo == 6) VanishNumBonus = 3;
	if (VanishPuyo == 7) VanishNumBonus = 4;
	if (VanishPuyo == 8) VanishNumBonus = 5;
	if (VanishPuyo == 9) VanishNumBonus = 6;
	if (VanishPuyo == 10) VanishNumBonus = 7;
	if (VanishPuyo >= 11) VanishNumBonus = 10;

	int AllBonus = ChainBonus + VanishNumBonus;
	if (AllBonus == 0) AllBonus = 1;

	PlusScore = VanishPuyo * 10 * AllBonus;

	ScoreCounter += PlusScore;
	AllVanishedNum += VanishPuyo;
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
	sprintf(Score, "%08d", (int)ScoreCounter);
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
		for (int j = 1; j < 8; j++)
		{
			if (map[i][j] == nullptr) continue;
			glPushMatrix();
			{
				glColor4f(map[i][j]->colorStatus[0], map[i][j]->colorStatus[1], map[i][j]->colorStatus[2], map[i][j]->colorStatus[3]);
				glTranslatef(-0.7 + (j - 1)*0.15, -0.75 + (i-1)*0.15, 1);
				glutSolidSphere(0.075, 180, 5);
			}
			glPopMatrix();
		}
	}

	return;
}



bool SceneIngame::Search_State_is(STATE _state)
{
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			if (map[i][j] != nullptr&&map[i][j]->_state == _state)
			{
				return true;
			}
		}
	}
	return false;
}

int SceneIngame::Search_States_are(STATE _state)
{
	int Puyos = 0;
	for (int i = 1; i < 15; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			if (map[i][j] == nullptr) continue;
			if (map[i][j] != nullptr&&map[i][j]->_state == _state)
			{
				Puyos++;
			}
		}
	}
	return Puyos;
}

