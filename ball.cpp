#include <iostream>
#include <algorithm>
#include <cmath>
#include <time.h>

#include "ball.h"
#include "ObjectTypes.h"

Ball::Ball() : Object()
{
	try
	{
		bTex = new Texture();
	}
	catch (std::string error)
	{
		throw(error);
	}

	currentPlayer = nullptr;
	shootDest = nullptr;
	ballState = FREE_STATE;
	shootCharge = 0;
	passCharge = false;
	dx = dy = 0;
	team = 0;
	delay = 0;
}

Ball::Ball(SDL_Renderer* renderer, SDL_Point* position, State state) : Object(position, 18, 18)
{
	bTex = new Texture(std::string("testAssets/ball.png"), renderer, position->x, position->y);
	goalTex = new Texture(std::string("testAssets/goalText.png"), renderer, 540, 260);
	currentPlayer = nullptr;
	shootDest = new SDL_Point { 0, 0 };
	ballState = state;
	shootCharge = 0;
	passCharge = false;
	dx = position->x;
	dy = position->y;
	team = 0;
	delay = 0;
	wasGoal = false;

	A.x = 0;
	A.y = 0;
	B.x = 0;
	B.y = 0;
}

Ball::~Ball()
{
	delete shootDest;
	delete bTex;
}

int Ball::getState(void)
{
	return (int)ballState;
}

void Ball::setCurrentPlayer(Player* p)
{
	currentPlayer = p;
	ballState = PLAYER_STATE;
}

Player* Ball::getCurrentPlayer(void)
{
	return currentPlayer;
}

bool Ball::hasDelay(void)
{
	return delay > 0;
}

void Ball::setState(State state)
{
	this->ballState = state;
}

void Ball::setPassCharge(bool charge)
{
	passCharge = charge;
}

void Ball::setShootCharge(int charge)
{
	shootCharge = charge;
}

void Ball::setNextPlayer(Player* nextPlayer)
{
	this->nextPlayer = nextPlayer;
}

void Ball::setDX(double dx)
{
	this->dx = dx;
}

void Ball::setDY(double dy)
{
	this->dy = dy;
}

void Ball::draw(SDL_Point* center, SDL_RendererFlip flip)
{

	bTex->setDimensions(Object::getPosition()->x, Object::getPosition()->y);
	bTex->renderTexture();

	if (wasGoal)
	{
		goalTex->renderTexture();
		wasGoal = false;
	}
}

Player* getClosestPlayer(SDL_Point* position, double angle, Object** obj, int team)
{
	Player* currentSelection = nullptr;
	double minDistance = 9999999;

	int start, finish;

	if (team == 1)
	{
		start = TEAM1_PLAYER2;
		finish = TEAM1_PLAYER5;
	}
	else
	{
		start = TEAM2_PLAYER2;
		finish = TEAM2_PLAYER5;
	}

	for (int i = start; i <= finish; ++i)
	{
		if (dynamic_cast<Player*>(obj[i])->isSelected() == false)
		{
			bool isViable = false;

			if (angle == 0 && obj[i]->getPosition()->x >= position->x) isViable = true;
			if ((angle == -180 || angle == 180) && obj[i]->getPosition()->x <= position->x) isViable = true;

			if (angle == 90 && obj[i]->getPosition()->y >= position->y) isViable = true;
			if (angle == -90 && obj[i]->getPosition()->y <= position->y) isViable = true;

			if (angle == 45 && obj[i]->getPosition()->x >= position->x && obj[i]->getPosition()->y >= position->y) isViable = true;
			if (angle == -45 && obj[i]->getPosition()->x >= position->x && obj[i]->getPosition()->y <= position->y) isViable = true;

			if (angle == 135 && obj[i]->getPosition()->x <= position->x && obj[i]->getPosition()->y >= position->y) isViable = true;
			if (angle == -135 && obj[i]->getPosition()->x <= position->x && obj[i]->getPosition()->y <= position->y) isViable = true;

			if (isViable)
			{
				double dist = distance(*position, { obj[i]->getPosition()->x,  obj[i]->getPosition()->y });
				if (minDistance > dist)
				{
					minDistance = dist;
					currentSelection = dynamic_cast<Player*>(obj[i]);
				}
			}
		}
	}

	return currentSelection;
}

int getAdjacent(SDL_Rect* ballColision, Object** obj, Player* current = NULL, int team = 0)
{
	Player* ret = nullptr;
	SDL_Rect* PlayerColision = new SDL_Rect;

	if (current == nullptr || team == 2)
	{
		for (int i = TEAM1_PLAYER1; i <= TEAM1_PLAYER5; ++i)
		{
			if (obj[i] != current)
			{
				PlayerColision->x = obj[i]->getPosition()->x;
				PlayerColision->y = obj[i]->getPosition()->y;
				PlayerColision->w = obj[i]->getWidth();
				PlayerColision->h = obj[i]->getHeight();
				if (SDL_HasIntersection(ballColision, PlayerColision))
				{
					return i;
				}
			}
		}
	}

	if (current == nullptr || team == 1)
	{
		for (int i = TEAM2_PLAYER1; i <= TEAM2_PLAYER5; ++i)
		{
			PlayerColision->x = obj[i]->getPosition()->x;
			PlayerColision->y = obj[i]->getPosition()->y;
			PlayerColision->w = obj[i]->getWidth();
			PlayerColision->h = obj[i]->getHeight();
			if (SDL_HasIntersection(ballColision, PlayerColision))
			{
				return i;
			}
		}
	}

	delete PlayerColision;

    return 0;
}

int getCurrentPlayerTeam(Player* currentPlayer, Object** obj)
{
	if(currentPlayer)
		return obj[TEAM2_PLAYER1] - dynamic_cast<Object*>(currentPlayer) > 0 ? 1 : 2;
	return 0;
}

void Ball::update(SDL_Event* e, const Uint8* keyboardState, Object** obj, bool* flag)
{

	if (willBeDelay)
	{
		SDL_Delay(2000);
		willBeDelay = false;
	}

	SDL_Point ballDest{ 0, 0 };
	
	if (ballState == PLAYER_STATE)
	{
		if ((keyboardState[SDL_SCANCODE_L] && currentPlayer->getTeam() == 1) || (keyboardState[SDL_SCANCODE_KP_3] && currentPlayer->getTeam() == 2))
		{
			passCharge = false;
			++shootCharge;
			if (shootCharge >= 30)
				shootCharge = 30;

			currentPlayer->setShootCharge(shootCharge);
		}
		else
		{
			if (shootCharge)
			{
				speed = (double)shootCharge;
				currentPlayer->setShootCharge(0);
				ballState = SHOOT_STATE;

				bTex->setAngle(currentPlayer->getTexture()->getAngle());

				if (bTex->getAngle() > -46 && bTex->getAngle() < 46)
				{
					shootDest->x = 1253;
					shootDest->y = 360;
				}
				else if (bTex->getAngle() < -134 || bTex->getAngle() > 134)
				{
					shootDest->x = 10;
					shootDest->y = 360;
				}
				else if ((int)bTex->getAngle() == 90)
				{
					shootDest->x = Object::getPosition()->x;
					shootDest->y = 720;
				}
				else
				{
					shootDest->x = Object::getPosition()->x;
					shootDest->y = 0;
				}

				if (shootDest->x == 24 || shootDest->x == 1253)
				{
					long double dx = shootDest->x - currentPlayer->getPosition()->x;
					long double dy = shootDest->y - currentPlayer->getPosition()->y;
					double angle = (100 * PI) / 380;
					angle = (angle * shootCharge) / 30;

					long double d1 = dy * dy - cos(angle / 2) * cos(angle / 2) * (dx * dx + dy * dy);
				    long double d2 = 2 * dy * dx * dx;
					long double d3 = pow(dx, 4) - cos(angle / 2) * cos(angle / 2) * (pow(dx, 4) + dx * dx * dy * dy);

					long double delta = d2 * d2 - 4 * d1 * d3;
					long double a1 = (-1 * d2 + sqrt(delta)) / (2 * d1);
					long double a2 = (-1 * d2 - sqrt(delta)) / (2 * d1);

					A.x = shootDest->x;
					B.x = shootDest->x;

					A.y = (int)(a1 < a2? a1 : a2) + currentPlayer->getPosition()->y;
					B.y = (int)(a1 > a2 ? a1 : a2) + currentPlayer->getPosition()->y;

					srand(time(NULL));
					shootDest->y = rand() % (B.y - A.y) + A.y;

					if ((shootDest->y >= 0 && shootDest->y <= 0))
						shootOnGoal = true;
					else
						shootOnGoal = false;

					distanceToGoal = sqrt(dx * dx + dy * dy);
				}

				shootChargeProcent = (double)shootCharge / 30;
				shootCharge = 0;

			}
		}

		if ((keyboardState[SDL_SCANCODE_K] && currentPlayer->getTeam() == 1) || (keyboardState[SDL_SCANCODE_KP_2] && currentPlayer->getTeam() == 2))
		{
			shootCharge = 0;
			currentPlayer->setShootCharge(0);
			passCharge = true;
		}
		else
		{
			if (passCharge)
			{
				passCharge = 0;
				speed = 8;
				
				nextPlayer = getClosestPlayer(Object::getPosition(), (int)currentPlayer->getTexture()->getAngle(), obj, currentPlayer->getTeam());

				if (nextPlayer)
				{
					ballState = PASS_STATE;
				}
			}
		}
	}

	switch (ballState)
	{
	case FREE_STATE:
	{
		SDL_Rect* ballColision = new SDL_Rect{ Object::getPosition()->x, Object::getPosition()->y, 18, 18 };
		int poz = getAdjacent(ballColision, obj);
		delete ballColision;

		if (poz > 0)
		{
			int start, finish;

			if (dynamic_cast<Player*>(obj[poz])->getTeam() == 1)
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
				dynamic_cast<Player*>(obj[i])->setSelection(false);

			dynamic_cast<Player*>(obj[poz])->setSelection(true);


			ballState = PLAYER_STATE;
			currentPlayer = dynamic_cast<Player*>(obj[poz]);
			delay = 45;
		}

		break;
	}
	case PLAYER_STATE:
	{
		if (delay > 0) --delay;

		team = currentPlayer->getTeam();

		SDL_Rect* ballColision = new SDL_Rect{ Object::getPosition()->x, Object::getPosition()->y, 18, 18 };
		int poz = getAdjacent(ballColision, obj, currentPlayer, team);
		delete ballColision;

		if (poz > 0 && delay == 0)
		{
			int start, finish;

			if (dynamic_cast<Player*>(obj[poz])->getTeam() == 1)
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
				dynamic_cast<Player*>(obj[i])->setSelection(false);

			dynamic_cast<Player*>(obj[poz])->setSelection(true);
			currentPlayer = dynamic_cast<Player*>(obj[poz]);
			delay = 45;
		}
		else
		{
			if (currentPlayer != NULL)
			{
				int addX, addY;
				int playerAngle = currentPlayer->getTexture()->getAngle();

				switch (playerAngle)
				{
				case 0:
					addX = 36;
					addY = 14;
					break;
				case 45:
					addX = 30;
					addY = 30;
					break;
				case 90:
					addX = 5;
					addY = 36;
					break;
				case 135:
					addX = -15;
					addY = 25;
					break;
				case 180:
					addX = -18;
					addY = 5;
					break;
				case -45:
					addX = 36;
					addY = -5;
					break;
				case -90:
					addX = 15;
					addY = -18;
					break;
				case -135:
					addX = -5;
					addY = -15;
					break;
				default:
					addX = 0;
					addY = 0;
					break;
				}

				if (currentPlayer->isMoving())
					bTex->setAngle(bTex->getAngle() + 10);

				dx = currentPlayer->getPosition()->x + addX;
				dy = currentPlayer->getPosition()->y + addY;
			}
		}
		break;
	}
	case SHOOT_STATE:
	{
		SDL_Rect* ballColision = new SDL_Rect{ Object::getPosition()->x, Object::getPosition()->y, 18, 18 };
		int poz = getAdjacent(ballColision, obj, currentPlayer, team);
		delete ballColision;

		if (poz > 0)
		{
			int start, finish;

			if (dynamic_cast<Player*>(obj[poz])->getTeam() == 1)
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
				dynamic_cast<Player*>(obj[i])->setSelection(false);

			dynamic_cast<Player*>(obj[poz])->setSelection(true);

			ballState = PLAYER_STATE;
			speed = 0;
			currentPlayer = dynamic_cast<Player*>(obj[poz]);
			delay = 45;
		}
		else
		{
			double nextPosX = dx, nextPosY = dy;
			
			updateCoord(shootDest, speed, nextPosX, nextPosY);

			if (nextPosX > 10 && nextPosX < 1245 && nextPosY > 0 && nextPosY < 705)
			{
				dx = nextPosX;
				dy = nextPosY;
				if (speed > 0)
				{
					speed -= 0.0001;
					if (speed < 0.001) speed = 0;
				}

				if (speed)
					bTex->setAngle(bTex->getAngle() + 5);
				else
					ballState = FREE_STATE;
			}
			else
			{
				int team2;
				bool goal = false;
				bool out = false;

				if (nextPosX <= 10)
				{
					dx = 5;
					team2 = 1;
				}
				if (nextPosX >= 1245)
				{
					team2 = 2;
					dx = 1245;
				}
				if (nextPosY <= 0) 
				{ 
					team2 = getCurrentPlayerTeam(currentPlayer, obj);
					out = true;
					dy = -5; 
				}
				if (nextPosY >= 705)
				{
					team2 = getCurrentPlayerTeam(currentPlayer, obj);
					out = true;
					dy = 705;
				}
				speed = 0;

				double goalProbability = 0.30 * (1 - distanceToGoal / 600) + 0.50 * shootChargeProcent;
				shootOnGoal = shootDest->y >= 300 && shootDest->y <= 400;
				
				goal = shootOnGoal && (rand() % 1000 < goalProbability * 1000);

				if (goal || out)
				{
					ballState = FREE_STATE;

					if (goal)
					{
						wasGoal = true;

						if (shootDest->x <= 300)
							flag[TEAM2_GOAL_FLAG] = true;
						else
							flag[TEAM1_GOAL_FLAG] = true;
					}

					SDL_Delay(100);
					resetPosition(obj, team2, goal, out);
					willBeDelay = true;
				}
				else
				{
					if (shootDest->x >= 600)
					{
						for (int i = TEAM2_PLAYER2; i <= TEAM2_PLAYER5; ++i)
							dynamic_cast<Player*>(obj[i])->setSelection(false);
						dynamic_cast<Player*>(obj[TEAM2_PLAYER1])->setSelection(true);

						currentPlayer = dynamic_cast<Player*>(obj[TEAM2_PLAYER1]);
						ballState = PLAYER_STATE;
					}
					else
					{
						for (int i = TEAM1_PLAYER2; i <= TEAM1_PLAYER5; ++i)
							dynamic_cast<Player*>(obj[i])->setSelection(false);
						dynamic_cast<Player*>(obj[TEAM1_PLAYER1])->setSelection(true);

						currentPlayer = dynamic_cast<Player*>(obj[TEAM1_PLAYER1]);
						ballState = PLAYER_STATE;
					}
				}
			}
		}
		break;
	}
	case PASS_STATE:
	{
		SDL_Rect* ballColision = new SDL_Rect{ Object::getPosition()->x, Object::getPosition()->y, 18, 18 };
		int poz = getAdjacent(ballColision, obj, currentPlayer, team);
		delete ballColision;

		if (poz > 0)
		{
			int start, finish;

			if (dynamic_cast<Player*>(obj[poz])->getTeam() == 1)
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
				dynamic_cast<Player*>(obj[i])->setSelection(false);

			dynamic_cast<Player*>(obj[poz])->setSelection(true);

			ballState = PLAYER_STATE;
			speed = 0;
			currentPlayer = dynamic_cast<Player*>(obj[poz]);
			delay = 45;
		}
		else
		{
			double nextPosX = dx, nextPosY = dy;

			updateCoord(nextPlayer->getPosition(), speed, nextPosX, nextPosY);

			if (abs(nextPosX - nextPlayer->getPosition()->x) > 5 || abs(nextPosY - nextPlayer->getPosition()->y) > 5)
			{
				dx = nextPosX;
				dy = nextPosY;
			}
			else
			{
				if (currentPlayer->isSelected())
				{
					currentPlayer->setSelection(false);
					nextPlayer->setSelection(true);
				}
				currentPlayer = nextPlayer;
				ballState = PLAYER_STATE;
			}

			bTex->setAngle(bTex->getAngle() + 5);
		}
		break;
	}
	default:
		break;
	}
	
	Object::setPosition(dx, dy);
}
