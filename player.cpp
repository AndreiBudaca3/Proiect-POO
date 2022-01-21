#include <iostream>
#include <cmath>

#include "ball.h"
#include "ObjectTypes.h"

Player::Player() : Object()
{
	currentFrame = 0;
	movingState = false;
	frameTime = 0;
	frameAdd = 1;
	verticalMovement = 0;
	horizontalMovement = 0;
	frameNumber = 3;
	spriteCoord[0] = { 200, 300, 36, 108 };
	spriteCoord[1] = { 0, 36, 37, 71 };
	spriteCoord[2] = { 0, 72, 37, 107 };
	dx = 0;
	dy = 0;

	try
	{
		pTex = new Texture();
	}
	catch (std::string err)
	{
		throw(err);
	}

	selected = false;
}

Player::Player(SDL_Renderer* renderer, int color, SDL_Point* position, int team, double angle, bool selected) : Object(position, 36, 38)
{
	char aux[3];
	if (color < 0 || color >= 13) color = 1;
	_itoa_s(color, aux, 10);

	try
	{
		pTex = new Texture(std::string("testAssets/playerSprite") + std::string(aux) + std::string(".png"), renderer, position->x, position->y, angle);
	}
	catch (std::string err)
	{
		throw (err);
		return;
	}

	currentFrame = 0;
	movingState = false;
	verticalMovement = 0;
	horizontalMovement = 0;
	frameNumber = 3;
	frameTime = 0;
	frameAdd = 1;
	spriteCoord[0] = { 0, 0, 36, 35 };
	spriteCoord[1] = { 0, 36, 37, 35 };
	spriteCoord[2] = { 0, 72, 37, 35 };
	dx = position->x;
	dy = position->y;
	this->selected = selected;
	this->team = team;
	switchDelay = 0;
	noUpdate = false;
	shootCharge = 0;
}

Player::~Player()
{
	delete pTex;
}

Texture* Player::getTexture(void)
{
	return pTex;
}

bool Player::isMoving(void)
{
	return movingState;
}

bool Player::isSelected(void)
{
	return selected;
}

void Player::setSelection(bool state)
{
	selected = state;
}

void Player::setMovingState(bool state)
{
	movingState = state;
}

void Player::setSwitchDelay(int value)
{
	switchDelay = value;
}

int Player::getSwitchDelay(void)
{
	return switchDelay;
}

int Player::getTeam(void)
{
	return team;
}

double Player::getDX(void)
{
	return dx;
}

double Player::getDY(void)
{
	return dy;
}

void Player::setDX(double dx)
{
	this->dx = dx;
}

void Player::setDY(double dy)
{
	this->dy = dy;
}

bool Player::willNotUpdate(void)
{
	return noUpdate;
}

void Player::setUpdate(bool state)
{
	noUpdate = !state;
}

void Player::resetAnimation(void)
{
	currentFrame = 1;
}

void Player::setShootCharge(int shootCharge)
{
	this->shootCharge = shootCharge;
}

void Player::draw(SDL_Point* center, SDL_RendererFlip flip)
{	
	if (movingState)
	{
		++frameTime;
		if (frameTime == 10)
		{
			currentFrame += frameAdd;
			frameTime = 0;
			if (currentFrame == frameNumber - 1)
			{
				frameAdd = -1;
			}
			if (currentFrame == 0)
			{
				frameAdd = 1;
			}
		}
	}
	else
	{
		currentFrame = 1;
	}

	if (Player::isSelected())
	{
		int x = Player::getPosition()->x;
		int y = Player::getPosition()->y;

		SDL_Point select[] = { {x + 18, y - 5}, {x + 10, y - 15}, {x + 26, y - 15}, {x + 18, y - 5} };

		if(team == 1)
			SDL_SetRenderDrawColor(Player::getTexture()->getRenderer(), 0xFF, 0x00, 0xFF, 0xFF);
		else
			SDL_SetRenderDrawColor(Player::getTexture()->getRenderer(), 0xFF, 0xA5, 0x00, 0xFF);

		SDL_RenderDrawLines(Player::getTexture()->getRenderer(), select, 4);

		if (shootCharge)
		{
			SDL_Rect greenCharge{Player::getPosition()->x, Player::getPosition()->y - 5, (shootCharge * 12) / 10 , 5 };
			SDL_Rect yellowCharge{ Player::getPosition()->x + 12, Player::getPosition()->y - 5, shootCharge > 10? ((shootCharge - 10) * 12) / 10 : 0 , 5 };
			SDL_Rect redCharge{ Player::getPosition()->x + 24, Player::getPosition()->y - 5, shootCharge > 20 ? ((shootCharge - 20) * 12) / 10 : 0 , 5 };
			SDL_Rect border{ Player::getPosition()->y - 5, (shootCharge * 36)/60 , 5 };

			SDL_SetRenderDrawColor(Player::getTexture()->getRenderer(), 0x00, 0x00, 0x00, 0xFF); //Black
			SDL_RenderDrawRect(Player::getTexture()->getRenderer(), &border);
			SDL_SetRenderDrawColor(Player::getTexture()->getRenderer(), 0x0F, 0xFF, 0x0F, 0xFF); //Green
			SDL_RenderFillRect(Player::getTexture()->getRenderer(), &greenCharge);
			SDL_SetRenderDrawColor(Player::getTexture()->getRenderer(), 0xFF, 0xFF, 0x00, 0xFF); //Yellow
			SDL_RenderFillRect(Player::getTexture()->getRenderer(), &yellowCharge);
			SDL_SetRenderDrawColor(Player::getTexture()->getRenderer(), 0xFF, 0x00, 0x00, 0xFF); //Red
			SDL_RenderFillRect(Player::getTexture()->getRenderer(), &redCharge);

		}
	}

	pTex->setDimensions(Object::getPosition()->x, Object::getPosition()->y);
	pTex->renderTexture(spriteCoord + currentFrame);
}

double distance(SDL_Point a, SDL_Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Player* getMarkingPlayer(Object* current, int team, Object** obj)
{
	int poz = 0;
	int holdPoz;

	for (int i = TEAM1_PLAYER2; i <= TEAM2_PLAYER5; ++i)
	{
		if (obj[i] == current)
			poz = i;
	}

	if (poz > TEAM2_PLAYER1)
		holdPoz = TEAM1_PLAYER2 + (poz - TEAM2_PLAYER2);
	else
		holdPoz = TEAM2_PLAYER2 + (poz - TEAM1_PLAYER2);

	if (poz == 0)
		return nullptr;

	if (team == 1)
	{
		//Check the player to be in the opposite half
		if (obj[holdPoz] != nullptr)
			return dynamic_cast<Player*>(obj[holdPoz]);

	}
	else
	{
		if (obj[holdPoz])
			return dynamic_cast<Player*>(obj[holdPoz]);
	}

	return nullptr;
}

double getTextureAngle(SDL_Point* currPos, SDL_Point* endPos)
{
	double Xdif = endPos->x - currPos->x;
	double Ydif = endPos->y - currPos->y;
	double angle = acos(Xdif / sqrt(Xdif * Xdif + Ydif * Ydif));

	angle = angle * 180 / PI;
	if (Ydif < 0) angle *= -1;

	if (angle > -22 && angle < 22) return 0;
	if (angle <= -22 && angle >= -67) return -45;
	if (angle < -67 && angle > -112) return -90;
	if (angle <= -112 && angle >= -158) return -135;
	if (angle < -158 || angle > 158) return -180;
	if (angle >= 22 && angle <= 67) return 45;
	if (angle > 67 && angle < 112) return 90;
	if (angle >= 112 && angle <= 158) return 135;

	return 0;
}

void updateCoord(SDL_Point* pointToGo, double speed, double& dx, double& dy)
{
	double angle;

	double xDif = pointToGo->x - dx;
	double yDif = pointToGo->y - dy;

	if (xDif != 0)
	{
		angle = atan((double)yDif / (double)xDif);


		if (xDif > 0)
		{
			dx += cos(angle) * speed;
			dy += sin(angle) * speed;
		}
		else
		{
			dx -= cos(angle) * speed;
			dy -= sin(angle) * speed;
		}
	}
	else
	{
		angle = 3.141527 / 2;


		if (yDif > 0)
		{
			dy += sin(angle) * speed;
		}
		else
		{
			dy -= sin(angle) * speed;
		}
	}
}

bool isClosestToBall(Object* current, Object** obj, int team)
{
	double minDist = 99999, dist;
	Object* minDistPlayer = nullptr;

	int start, finish;
	if (team == 1)
	{
		start = TEAM1_PLAYER2;
		finish = TEAM1_PLAYER4;
	}
	else
	{
		start = TEAM2_PLAYER2;
		finish = TEAM2_PLAYER4;
	}

	for (int i = start; i <= finish; ++i)
	{
		dist = distance(*(obj[i]->getPosition()), *(obj[BALL]->getPosition()));
		if (dist < minDist)
		{
			minDist = dist;
			minDistPlayer = obj[i];
		}

	}

	if (minDistPlayer == current)
		return true;
	return false;
}

int getActivePlayerTeam(Object** obj)
{
	for (int i = TEAM1_PLAYER1; i <= TEAM1_PLAYER5; ++i)
	{
		if (dynamic_cast<Player*>(obj[i])->isSelected()) return 1;
	}

	return 2;
}


