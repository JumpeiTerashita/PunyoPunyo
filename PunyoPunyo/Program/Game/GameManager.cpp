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

GameManager* GameManager::instance = nullptr;
SceneIngame* SceneIngame::instance = nullptr;

GameManager::GameManager() {
	HighScore = 877;
	KeyEventReset();
	
	pFile = fopen(
		"Brick.data", // const char * _Filename
		"rb");      // const char * _Mode

	if (!pFile) setSequence(&GameManager::Scene_ReadError);
	else
	{
		fread(
			pixels,        // void * _DstBuf
			300 * 300 * 3, // size_t _ElementSize
			1,          // size_t _Count
			pFile);     // FILE * _File


		fclose(pFile);// FILE * _File
		setSequence(&GameManager::Scene_Title);
	}
		
};

GameManager* GameManager::getInstance() {
	if (nullptr == GameManager::instance) {
		GameManager::instance = new GameManager();
	}
	return GameManager::instance;
}

void GameManager::Scene_ReadError()
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

	if (_Key)
	{
		exit(1);
	}
}

void GameManager::Scene_Title()
{
	glPushMatrix();
	{
		glColor4f(1, 1, 1, 1);
		glTranslatef(-0.45f, 0.3f, 0);
		glScalef(0.001f, 0.001f, 0.001f);
		DrawString_Stroke("Punyo Punyo");
	}
	glPopMatrix();
	glPushMatrix();
	{
		glColor4f(1, 1, 1, 1);
		glRasterPos2f(-0.32, -0.2);
		DrawString("PUSH  ANY  BUTTON");
	}
	glPopMatrix();

	if (_Key)
	{
		KeyEventReset();
		setSequence(&GameManager::Scene_Ingame);
	}
}

void GameManager::Scene_Ingame()
{
	objects.sort(sortPosYPriority);
	SceneIngame::getInstance()->update();

	SceneIngame* Scene = SceneIngame::getInstance();
	if (BitChecker(Scene->KeyFlag, KeyFlag_LEFT)) objects.sort(sortPosXPriority_Ascending);
	if (BitChecker(Scene->KeyFlag, KeyFlag_RIGHT)) objects.sort(sortPosXPriority_Descending);

	std::list< GameObject* >::iterator it = GameManager::getInstance()->objects.begin();
	
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
	SceneIngame* Scene = SceneIngame::getInstance();
	std::list< GameObject* >::iterator it = GameManager::getInstance()->objects.begin();

	while (it != objects.end()) {
		if (Scene->map[(*it)->pos._y][(*it)->pos._x+lookX] != nullptr) return false;
		it++;
	}

	return true;
}

void GameManager::update() {
	runSequence();
}

void GameManager::display() 
{

	if (getSequence() == &GameManager::Scene_Ingame)
	{
		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報） 残像出なくなる

		glLoadIdentity();

		SceneIngame::getInstance()->display();

		//objects.sort(sortRenderPriority);

		std::list< GameObject* >::iterator it = GameManager::getInstance()->objects.begin();

		while (it != objects.end()) {
			(*it)->ObjDisp();

			it++;
		}
		
	}
	
}









