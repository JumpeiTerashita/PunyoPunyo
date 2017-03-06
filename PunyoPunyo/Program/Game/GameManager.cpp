#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include <stdio.h>
#include "GameManager.h"
#include "../glut.h"
#include "../Engine/InputManager.h"
#include "Puyo.h"
#include "Scene_InGame.h"
#include "../Liblary/BitController.h"
#include "../Engine/DrawString.h"

GameManager* GameManager::Instance = nullptr;
SceneIngame* SceneIngame::Instance = nullptr;

GameManager::GameManager() {
	HighScore = 877;
	SelectDifficulty = 0;
	KeyEventReset();

	Brick = fopen(
		"Brick.data", // const char * _Filename
		"rb");      // const char * _Mode

	if (!Brick) setSequence(&GameManager::Scene_ReadError);
	else
	{
		fread(
			BrickPixels,        // void * _DstBuf
			300 * 300 * 3, // size_t _ElementSize
			1,          // size_t _Count
			Brick);     // FILE * _File


		fclose(Brick);// FILE * _File
		setSequence(&GameManager::Scene_Title);
	}
};

GameManager* GameManager::GetInstance() {
	if (nullptr == GameManager::Instance) {
		GameManager::Instance = new GameManager();
	}
	return GameManager::Instance;
}

void GameManager::Scene_ReadError()
{
	if (_Key)
	{
		exit(1);
	}
}

void GameManager::Scene_Title()
{
	SelectDifficulty = 0;
	if (_Key)
	{
		KeyEventReset();
		setSequence(&GameManager::Scene_DifficultySelect);
	}
}

void GameManager::Scene_DifficultySelect()
{
	if (_SpecialKey == GLUT_KEY_UP || _Key == 'w')
	{
		if (SelectDifficulty == EASY) SelectDifficulty = DIFFICULTY_NUM - 1;
		else SelectDifficulty--;
		KeyEventReset();
	}
	else if (_SpecialKey == GLUT_KEY_DOWN || _Key == 's')
	{
		if (SelectDifficulty == DIFFICULTY_NUM - 1) SelectDifficulty = EASY;
		else SelectDifficulty++;
		KeyEventReset();
	}
	else if (_Key)
	{
		KeyEventReset();
		SceneIngame::GetInstance()->SetDifficulty(SelectDifficulty);
		setSequence(&GameManager::Scene_Ingame);
	}
}

void GameManager::Scene_Ingame()
{
	objects.sort(sortPosYPriority);
	SceneIngame::GetInstance()->update();

	SceneIngame* Scene = SceneIngame::GetInstance();
	if (BitChecker(Scene->KeyFlag, KeyFlag_LEFT)) objects.sort(sortPosXPriority_Ascending);
	if (BitChecker(Scene->KeyFlag, KeyFlag_RIGHT)) objects.sort(sortPosXPriority_Descending);

	std::list< GameObject* >::iterator it = GameManager::GetInstance()->objects.begin();

	while (it != objects.end()) {
		(*it)->ObjUpdate();
		if (0 == (*it)->getLifeTime())
		{
			delete (*it);
			it = objects.erase(it);
			//system("cls");
			continue;
		}
		it++;
	}
	KeyEventReset();
}


bool GameManager::MoveSearch(int lookX)
{
	SceneIngame* Scene = SceneIngame::GetInstance();
	std::list< GameObject* >::iterator it = GameManager::GetInstance()->objects.begin();

	while (it != objects.end()) {
		if (Scene->Map[(*it)->pos._y][(*it)->pos._x + lookX] != nullptr) return false;
		it++;
	}

	return true;
}

void GameManager::update() {
	runSequence();
}

void GameManager::display()
{
	if (getSequence() == &GameManager::Scene_ReadError)
	{
		glPushMatrix();
		{
			glColor4f(1, 0, 0, 1);
			glTranslatef(-0.6f, 0.3f, 0);
			glScalef(0.001f, 0.001f, 0.001f);
			DrawString_Stroke("FILE READ ERROR!");
		}
		glPopMatrix();
		glPushMatrix();
		{
			glColor4f(1, 1, 1, 1);
			glRasterPos2f(-0.5, -0.2);
			DrawString("\"Brick.data\" not find.");
			glRasterPos2f(-0.5, -0.4);
			DrawString("Please place \"Brick.data\" in the same folder ");
			glRasterPos2f(-0.5, -0.5);
			DrawString("as the \"PunyoPunyo.exe\" file.");
		}
		glPopMatrix();
	}

	if (getSequence() == &GameManager::Scene_Title)
	{
		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報） 残像出なくなる

		glLoadIdentity();

		glPushMatrix();
		{
			glColor4f(1, 0, 0.5, 1);
			glLineWidth(7);
			glTranslatef(-0.6f, 0.6f, 0);
			glScalef(0.001f, 0.001f, 0.001f);
			DrawString_Stroke("P u n y o");
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor4f(1, 0, 0.5, 1);
			glLineWidth(7);
			glTranslatef(-0.2f, 0.35f, 0);
			glScalef(0.001f, 0.001f, 0.001f);
			DrawString_Stroke("P u n y o");
		}
		glPopMatrix();



		glPushMatrix();
		{
			glColor4f(1, 1, 1, 1);
			glLineWidth(3);
			glTranslatef(-0.625f, -0.6f, 0);
			glScalef(0.0005f, 0.0005f, 0.0005f);
			DrawString_Stroke("P U S H  A N Y  B U T T O N");
		}
		glPopMatrix();
	}

	if (getSequence() == &GameManager::Scene_DifficultySelect)
	{
		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報） 残像出なくなる

		glLoadIdentity();

		glPushMatrix();
		{
			glColor4f(1, 0, 0.5, 1);
			glLineWidth(7);
			glTranslatef(-0.6f, 0.6f, 0);
			glScalef(0.001f, 0.001f, 0.001f);
			DrawString_Stroke("P u n y o");
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor4f(1, 0, 0.5, 1);
			glLineWidth(7);
			glTranslatef(-0.2f, 0.35f, 0);
			glScalef(0.001f, 0.001f, 0.001f);
			DrawString_Stroke("P u n y o");
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor4f(1, 1, 1, 1);
			glLineWidth(3);
			glTranslatef(-0.7f, 0, 0);
			glScalef(0.0006f, 0.0006f, 0.001f);
			DrawString_Stroke("Choose Difficulty");
		}
		glPopMatrix();

		for (int i = 0; i < DIFFICULTY_NUM; i++)
		{
			glPushMatrix();
			{
				if (i == SelectDifficulty) glColor4f(1, 1, 0, 1);
				else glColor4f(1, 1, 1, 1);
				glRasterPos2f(0.1, -0.1*i);
				DrawString(DifficultyDisp[i]);
			}
			glPopMatrix();
			char PuyoCOLORs[2];
			sprintf(PuyoCOLORs, "%d", i + 3);
			if (i == SelectDifficulty)
			{
				glPushMatrix();
				{
					glColor4f(1, 1, 1, 1);
					glRasterPos2f(-0.7, -0.6);
					DrawString(DifficultyDisp[i]);
					glRasterPos2f(-0.2, -0.6);
					DrawString("Puyos COLOR  = ");
					glRasterPos2f(0.3, -0.6);
					DrawString(PuyoCOLORs);
				}
				glPopMatrix();

				for (int j = 0; j < SelectDifficulty + 3; j++)
				{
					glPushMatrix();
					{
						GameManager::glColorSetup(j);
						glTranslatef(-0.125 + 0.15*j, -0.75, 1);
						glutSolidSphere(0.075, 180, 5);
					}
					glPopMatrix();
				}
			}
		}
	}


	if (getSequence() == &GameManager::Scene_Ingame)
	{
		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報） 残像出なくなる

		glLoadIdentity();

		SceneIngame::GetInstance()->display();

		std::list< GameObject* >::iterator it = GameManager::GetInstance()->objects.begin();

		while (it != objects.end()) {
			(*it)->ObjDisp();

			it++;
		}

	}

}

void GameManager::glColorSetup(int _ColorNumber)
{
	switch (_ColorNumber)
	{
	case COLOR_RED:
		glColor4f(1, 0, 0, 1);
		break;
	case COLOR_BLUE:
		glColor4f(0, 0.3, 1, 1);
		break;
	case COLOR_GREEN:
		glColor4f(0, 1, 0, 1);
		break;
	case COLOR_YELLOW:
		glColor4f(1, 1, 0, 1);
		break;
	case COLOR_PURPLE:
		glColor4f(1, 0, 1, 1);
		break;
	case COLOR_LIGHTBLUE:
		glColor4f(0.7f, 0.9f, 1, 1);
		break;
	case COLOR_ORANGE:
		glColor4f(1, 0.6f, 0, 1);
		break;
	}
}









