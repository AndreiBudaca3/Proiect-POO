#include "object.h"

#pragma once

enum Direction
{
	RIGHT,
	UP_LEFT,

};

class Player : public Object
{
private:
	int currentFrame;
	int frameAdd;
	int frameNumber;
	int frameTime;
	int team;
	bool movingState;
	int verticalMovement;
	int horizontalMovement;
	double dx, dy;
	bool selected;
	int switchDelay;
	bool noUpdate;
	int shootCharge;

	SDL_Rect spriteCoord[3];
	Texture* pTex;

public:
	Player();
	Player(SDL_Renderer* renderer, int color, SDL_Point* position, int team, double angle = 0, bool selected = false);
	virtual ~Player();

	Texture* getTexture(void);
	bool isMoving(void);
	bool isSelected(void);
	void setSelection(bool state);
	void setMovingState(bool state);
	void setSwitchDelay(int value);
	int getSwitchDelay(void);
	int getTeam(void);
	double getDX(void);
	double getDY(void);
	void setDX(double dx);
	void setDY(double dy);
	bool willNotUpdate(void);
	void setUpdate(bool state);
	void resetAnimation(void);
	void setShootCharge(int shootCharge);

	void draw(SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	virtual void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL) = 0;
};

double distance(SDL_Point a, SDL_Point b);
Player* getMarkingPlayer(Object* current, int team, Object** obj);
double getTextureAngle(SDL_Point* currPos, SDL_Point* endPos);
void updateCoord(SDL_Point* pointToGo, double speed, double& dx, double& dy);
bool isClosestToBall(Object* current, Object** obj, int team);
int getActivePlayerTeam(Object** obj);