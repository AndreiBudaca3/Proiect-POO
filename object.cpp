#include <iostream>

#include "ball.h"
#include "ObjectTypes.h"

Object::Object()
{
	position = nullptr;
	w = h = 0;
}

Object::Object(SDL_Point* position, int width, int height)
{
	this->position = position;
	w = width;
	h = height;
}

Object::~Object()
{
	std::cout << "Obiect distrus!\n";
	if (position != nullptr)
	{
		delete position;
		position = nullptr;
	}
}

SDL_Point* Object::getPosition(void)
{
	if(this)
		return position;
}

void Object::setPosition(int x, int y)
{
	position->x = x;
	position->y = y;
}

int Object::getWidth(void)
{
	return w;
}

void Object::setWidth(int w)
{
	this->w = w;
}

int Object::getHeight(void)
{
	return h;
}

void Object::setHeight(int h)
{
	this->h = h;
}

void resetPosition(Object** objVec, int team, bool goal, bool out)
{

	if (goal)
	{
		double dx[] = { 50, 265, 265, 620, 620, 1200, 725, 725, 1000, 1000 };
		double dy[] = { 340, 145, 530, 285, 380, 340, 200, 480, 145, 530 };
		double angle[] = { 0, 0, 0, 90, -90, -180, -180, -180, -180, -180 };
		int shift = TEAM1_PLAYER1;

		if (team == 2)
		{
			dx[TEAM2_PLAYER2 - shift] = 620;
			dy[TEAM2_PLAYER2 - shift] = 380;
			angle[TEAM2_PLAYER2 - shift] = -90;
			dx[TEAM2_PLAYER3 - shift] = 620;
			dy[TEAM2_PLAYER3 - shift] = 285;
			angle[TEAM2_PLAYER3 - shift] = 90;

			dx[TEAM1_PLAYER4 - shift] = 500;
			dy[TEAM1_PLAYER4 - shift] = 200;
			angle[TEAM1_PLAYER4 - shift] = 0;
			dx[TEAM1_PLAYER5 - shift] = 500;
			dy[TEAM1_PLAYER5 - shift] = 510;
			angle[TEAM1_PLAYER5 - shift] = 0;
		}

		dynamic_cast<Ball*>(objVec[BALL])->setDX(630);
		dynamic_cast<Ball*>(objVec[BALL])->setDY(345);

		for (int i = TEAM1_PLAYER1; i <= TEAM2_PLAYER5; ++i)
		{
			Player* aux = dynamic_cast<Player*>(objVec[i]);
			if (aux)
			{
				aux->setDX(dx[i - TEAM1_PLAYER1]);
				aux->setDY(dy[i - TEAM1_PLAYER1]);
				aux->setPosition(dx[i - TEAM1_PLAYER1], dy[i - TEAM1_PLAYER1]);
				aux->getTexture()->setAngle(angle[i - TEAM1_PLAYER1]);
				aux->getTexture()->setDimensions(dx[i - TEAM1_PLAYER1], dy[i - TEAM1_PLAYER1]);
				aux->setSelection(false);
				aux->resetAnimation();
				aux->setUpdate(false);
			}

			dynamic_cast<Player*>(objVec[TEAM1_PLAYER4])->setSelection(true);
			dynamic_cast<Player*>(objVec[TEAM2_PLAYER2])->setSelection(true);
		}
	}
	else
	{
		int start;
		int finish;
		Player* throwPlayer = NULL;

		if (team == 1)
		{
			start = TEAM1_PLAYER1;
			finish = TEAM1_PLAYER5;
		}
		else
		{
			start = TEAM2_PLAYER1;
			finish = TEAM2_PLAYER5;
		}

		for (int i = start; i <= finish; ++i)
			if (dynamic_cast<Player*>(objVec[i])->isSelected())
				throwPlayer = dynamic_cast<Player*>(objVec[i]);

		if (throwPlayer == NULL)
		{
			if (team == 1)
				throwPlayer = dynamic_cast<Player*>(objVec[TEAM1_PLAYER4]);
			else
				throwPlayer = dynamic_cast<Player*>(objVec[TEAM2_PLAYER3]);
		}


		throwPlayer->setDX(objVec[BALL]->getPosition()->x - 9);
		throwPlayer->setDY(objVec[BALL]->getPosition()->y - 9);
		throwPlayer->setPosition(objVec[BALL]->getPosition()->x - 9, objVec[BALL]->getPosition()->y - 9);
		throwPlayer->getTexture()->setAngle(objVec[BALL]->getPosition()->y < 300? 90 : -90);
		throwPlayer->getTexture()->setDimensions(objVec[BALL]->getPosition()->x - 9, objVec[BALL]->getPosition()->y - 9);
		throwPlayer->resetAnimation();
		throwPlayer->setUpdate(false);
	}
}
