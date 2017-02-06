#include <list>
#include <stdio.h>
#include "glut.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Puyo.h"
#include "Scene_InGame.h"

GameManager* GameManager::instance = nullptr;
SceneIngame* SceneIngame::instance = nullptr;


void DrawString_Stroke(const char* str)
{
	while (*str != '\0')
	{

		glutStrokeCharacter(
			GLUT_STROKE_ROMAN,  // void *font
			*str++);               // int character
	}
}

extern void DrawString(const char* str);


GameManager::GameManager() {
	HighScore = 877;
	KeyEventReset();
	setSequence(&GameManager::Scene_Title);
};

GameManager* GameManager::getInstance() {
	if (nullptr == GameManager::instance) {
		GameManager::instance = new GameManager();
	}
	return GameManager::instance;
}

void GameManager::Scene_Title()
{
	glPushMatrix();
	{
		glColor4f(1, 1, 1, 1);
		glTranslatef(-0.45f, 0.3f, 0);
		glScalef(0.001f, 0.001f, 0.001f);
		DrawString_Stroke("P U Y O !");
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
	SceneIngame::getInstance()->update();

	std::list< GameObject* >::iterator it = GameManager::getInstance()->objects.begin();

	while (it != objects.end()) {
		//printf("%d\n", objects.size());



		(*it)->ObjUpdate();


		if (0 == (*it)->getLifeTime())
		{
			delete (*it);
			it = objects.erase(it);
			continue;
		}
		it++;
	}

	KeyEventReset();
}

void GameManager::update() {
	runSequence();
}

void GameManager::display() {
	//if (gameState == SCENE_TITLE)
	//{
	//	glMatrixMode(GL_PROJECTION);// GLenum mode
	//	glLoadIdentity();
	//	gluOrtho2D(
	//		0,      // GLdouble left
	//		300,    // GLdouble right
	//		0,      // GLdouble bottom
	//		300);   // GLdouble top

	//	glMatrixMode(GL_MODELVIEW);// GLenum mode
	//	glLoadIdentity();

	//	glutStrokeCharacter(
	//		GLUT_STROKE_ROMAN,  // void *font
	//		'A');               // int character
	//}


	if (getSequence() == &GameManager::Scene_Ingame)
	{

		glClear(GL_COLOR_BUFFER_BIT);//クリア（色情報） 残像出なくなる

		glLoadIdentity();

		SceneIngame::getInstance()->display();

		std::list< GameObject* >::iterator it = GameManager::getInstance()->objects.begin();

		while (it != objects.end()) {
			(*it)->ObjDisp();

			it++;
		}
		
	}
	
}

//void GameManager::SceneChanger() {
//	if (_Key=='z') 
//	{
//		gameState++;
//		if (gameState >= SCENE_MAX) gameState = SCENE_TITLE;
//		
//	}
//}
//
//void GameManager::SceneFactory() {
//	//if (_SpecialKey == GLUT_KEY_RIGHT) //トリガー例
//	//{
//	//	gameState++;
//	//	if (gameState >= SCENE_MAX) gameState = SCENE_TITLE;
//	//}
//}










