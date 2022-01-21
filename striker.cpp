#include <iostream>

#include "striker.h"
#include "ball.h"
#include "ObjectTypes.h"

Striker::Striker() : Player()
{
	speed = 0;
	isMarking = false;
	pointToGo = nullptr;
	initialPosition = nullptr;
}

Striker::Striker(SDL_Renderer* renderer, int teamColor, SDL_Point* position, int team, int difficulty, double angle, bool selected) :
	Player(renderer, teamColor, position, team, angle, selected)
{
	speed = 3 - difficulty * 0.5;
	isMarking = false;
	pointToGo = new SDL_Point;
	initialPosition = new SDL_Point{ position->x, position->y };
}

Striker::~Striker()
{
	delete pointToGo;
	delete initialPosition;
}

SDL_Point* Striker::getPointToGo(void)
{
	return pointToGo;
}

void Striker::update(SDL_Event* e, const Uint8* keyboardState, Object** obj, bool* flag)
{
	if (Player::willNotUpdate())
	{
		Player::setUpdate(true);
		return;
	}

	double dx = Player::getDX();
	double dy = Player::getDY();

	//If the player is the one selected, it will go acordingly to the user inputs
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

	//Else, the player will use "AI" to move
	else
	{
		Player* playerToFollow = getMarkingPlayer(dynamic_cast<Object*>(this), Player::getTeam(), obj);

		//Ball is in FREE, SHOOT or PASS state, or the player's team has the ball
		if (dynamic_cast<Ball*>(obj[BALL])->getState() != PLAYER_STATE || dynamic_cast<Ball*>(obj[BALL])->getCurrentPlayer() == nullptr || dynamic_cast<Ball*>(obj[BALL])->getCurrentPlayer()->getTeam() == Player::getTeam())
		{
			//Ball is in FREE / SHOOT state. If the striker is the closest in his team, he will go towards the ball
			if (dynamic_cast<Ball*>(obj[BALL])->getState() != PLAYER_STATE)
			{
				if (isClosestToBall(dynamic_cast<Object*>(this), obj, Player::getTeam()))
				{
					updateCoord(obj[BALL]->getPosition(), speed, dx, dy);
					Player::getTexture()->setAngle(getTextureAngle(Player::getPosition(), obj[BALL]->getPosition()));
					Player::setMovingState(true);
				}
				else
				{
					Player::setMovingState(false);
				}
			}
			//Ball is on player's team.
			else
			{
				//Striker doesn't have the ball. Attacker will run toward the goal
				if (dynamic_cast<Ball*>(obj[BALL])->getCurrentPlayer() != this)
				{
					if (Player::getTeam() == 1)
					{
						pointToGo->x = 950;
						pointToGo->y = initialPosition->y;
					}
					else
					{
						pointToGo->x = 280;
					}

					pointToGo->y = initialPosition->y;

					if (abs(Player::getPosition()->x - pointToGo->x) > 5 || abs(Player::getPosition()->x - pointToGo->x) > 5)
					{
						updateCoord(pointToGo, speed, dx, dy);
						Player::getTexture()->setAngle(getTextureAngle(Player::getPosition(), pointToGo));
						Player::setMovingState(true);
					}
					else
					{
						Player::getTexture()->setAngle(Player::getTeam() == 1 ? 0 : -180);
						Player::setMovingState(false);
					}
				}

				//Striker has the ball
				else
				{
					setMovingState(false);
				}
			}
		}
		//The enemy teams has the ball
		else
		{
			if (playerToFollow != NULL)
			{
				//The defender has a player to follow (enemy attacker is in the player's half)
				if ((Player::getTeam() == 1 && playerToFollow->getPosition()->x < 650) || (Player::getTeam() == 2 && playerToFollow->getPosition()->x >= 550))
				{
					//The player to follow has the ball
					if (dynamic_cast<Ball*>(obj[BALL])->getCurrentPlayer() == playerToFollow)
					{
						//The defender will follow the ball position

						updateCoord(obj[BALL]->getPosition(), 2.5, dx, dy);
						Player::setMovingState(true);
						Player::getTexture()->setAngle(getTextureAngle(Player::getPosition(), obj[BALL]->getPosition()));
					}

					//The player to follow is in the enemy
					else
					{
						//The defender will stay between the ball and the player to hold


						double Xdif = dynamic_cast<Ball*>(obj[BALL])->getPosition()->x - playerToFollow->getPosition()->x;
						double Ydif = dynamic_cast<Ball*>(obj[BALL])->getPosition()->y - playerToFollow->getPosition()->y;

						//SDL_Point pointToGo;
						double q = sqrt(Xdif * Xdif + Ydif * Ydif);
						double t = acos(-1 * Xdif / q);

						if (Ydif > 0)
							t *= -1;

						pointToGo->x = (q - 80) * cos(t) + dynamic_cast<Ball*>(obj[BALL])->getPosition()->x;
						pointToGo->y = (q - 80) * sin(t) + dynamic_cast<Ball*>(obj[BALL])->getPosition()->y;

						Xdif = (double)pointToGo->x - Object::getPosition()->x;
						Ydif = (double)pointToGo->y - Object::getPosition()->y;
						double dist = sqrt(Xdif * Xdif + Ydif * Ydif);

						if (dist >= 65 || isMarking == true)
						{
							if (dist < 25)
								isMarking = false;
							else
							{
								isMarking = true;

								updateCoord(pointToGo, 2.5, dx, dy);
								Player::getTexture()->setAngle(getTextureAngle(Player::getPosition(), pointToGo));
							}

							Player::setMovingState(isMarking);
						}
						else
						{
							Player::setMovingState(false);
							Player::getTexture()->setAngle(Player::getTeam() == 1 ? 0 : 180);
						}
					}
				}
				//The player to follow is in the oposite half
				else
				{
					//The defender will coppy the enemy Y coord, and will go towards the middle of the pitch on the X coord

					SDL_Point* pointToGo = new SDL_Point{ 450 , playerToFollow->getPosition()->y };

					if (Player::getTeam() == 2) pointToGo->x = 750;

					if (abs(Player::getPosition()->x - pointToGo->x) >= 25 || abs(Player::getPosition()->y - pointToGo->y) >= 25)
					{
						updateCoord(pointToGo, speed, dx, dy);
						Player::setMovingState(true);
					}
					else
						Player::setMovingState(false);

					delete pointToGo;
				}
			}
			else
			{
				Player::setMovingState(false);
			}
		}
	}

	if (dx < 40) dx = 40;
	if (dx > 1220) dx = 1220;
	if (dy > 675) dy = 675;
	if (dy < 0) dy = 0;

	Player::setDX(dx);
	Player::setDY(dy);
	Object::setPosition((int)dx, (int)dy);
	Player::getTexture()->setDimensions(Object::getPosition()->x, Object::getPosition()->y);
}
