#include <iostream>

#include "goalkeeper.h"
#include "ball.h"

Goalkeeper::Goalkeeper() : Player()
{
	dx = 0;
	dy = 0;
	horizontalMovement = 0;
	verticalMovement = 0;
	speed = 0;
}

Goalkeeper::Goalkeeper(SDL_Renderer* renderer, SDL_Point* position, int team, double angle, bool selected)
	: Player(renderer, 0, position, team, angle, selected)
{
	dx = position->x;
	dy = position->y;
	horizontalMovement = 0;
	verticalMovement = 0;
	speed = 1.5;

	initialPosition = new SDL_Point{position->x, position->y};
	pointToGo = new SDL_Point;
}

Goalkeeper::~Goalkeeper()
{
	delete pointToGo;
	delete initialPosition;
}

int getBallHalf(Object* ball)
{
	if (ball->getPosition()->x <= 630)
		return 1;
	return 2;
}

void Goalkeeper::update(SDL_Event* e, const Uint8* keyboardState, Object** obj, bool* flag)
{
	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		flag[PAUSE_FLAG] = true;
	}


	if (Player::isSelected())
	{
		Player::setMovingState(false);
		if ((keyboardState[SDL_SCANCODE_E] && Player::getTeam() == 1) || (keyboardState[SDL_SCANCODE_KP_ENTER] && Player::getTeam() == 2))
		{
			if (Player::getSwitchDelay())
				Player::setSwitchDelay(Player::getSwitchDelay() - 1);
			else
			{
				if (dynamic_cast<Player*>(this) != dynamic_cast<Ball*>(obj[BALL])->getCurrentPlayer())
				{
					Player* next = getClosestPlayer(getPosition(), getTexture()->getAngle(), obj, getTeam());

					if (next != NULL)
					{
						setSelection(false);
						next->setSelection(true);
						next->setSwitchDelay(15);
					}
				}
			}
		}
		else
		{
			horizontalMovement = false;
			verticalMovement = false;


			if (Player::getTeam() == 1)
			{
				horizontalMovement = (bool)keyboardState[SDL_SCANCODE_D] - (bool)keyboardState[SDL_SCANCODE_A];
				verticalMovement = (bool)keyboardState[SDL_SCANCODE_S] - (bool)keyboardState[SDL_SCANCODE_W];
			}
			else
			{
				horizontalMovement = (bool)keyboardState[SDL_SCANCODE_RIGHT] - (bool)keyboardState[SDL_SCANCODE_LEFT];
				verticalMovement = (bool)keyboardState[SDL_SCANCODE_DOWN] - (bool)keyboardState[SDL_SCANCODE_UP];
			}

			if (horizontalMovement != 0 || verticalMovement != 0)
			{
				Player::getTexture()->setAngle(acos(horizontalMovement / sqrt(horizontalMovement * horizontalMovement + verticalMovement * verticalMovement)) * (180.0 / 3.141592653589793238463));
				Player::setMovingState(true);
			}
			if (verticalMovement == -1)
				Player::getTexture()->setAngle(-1 * Player::getTexture()->getAngle());

			if (horizontalMovement && verticalMovement)
			{
				dx += horizontalMovement * speed * 0.71;
				dy += verticalMovement * speed * 0.71;
			}
			else
			{
				dx += horizontalMovement * speed;
				dy += verticalMovement * speed;
			}
		}
	}
	else
	{
		if (abs(dx - initialPosition->x) > 5 || abs(dy - initialPosition->y) > 5)
		{
			updateCoord(initialPosition, speed, dx, dy);
			SDL_Point currentPoz = { (int)dx, (int)dy };
			Player::getTexture()->setAngle(getTextureAngle(&currentPoz, pointToGo));
		}
		else
		{
			Player::setMovingState(false);
			if (getBallHalf(obj[BALL]) == Player::getTeam())
			{
				double X = obj[BALL]->getPosition()->x - Player::getPosition()->x;
				double Y = Player::getPosition()->y - obj[BALL]->getPosition()->y;
				double angle;

				if (X == 0 && Y == 0)
				{
					angle = 0;
				}
				else
				{
					angle = acos((X / sqrt(X * X + Y * Y))) * (180.0 / 3.141592653589793238463);
					if (Y > 0)
						angle *= -1;
				}


				Player::getTexture()->setAngle(angle);
			}
			else
				Player::getTexture()->setAngle(Player::getTeam() == 1 ? 0 : 180);
		}
	}

	Object::setPosition((int)dx, (int)dy);
}
