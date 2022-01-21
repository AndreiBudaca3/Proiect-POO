#include "goalkeeper.h"
#include "ObjectTypes.h"

#pragma once

class Ball : public Object
{
private:
	Texture* bTex;
	Texture* goalTex;
	Player* currentPlayer;
	Player* nextPlayer;
	State ballState;
	SDL_Point* shootDest;
	SDL_Point A;
	SDL_Point B;

	double speed;

	bool shootOnGoal;
	double distanceToGoal;
	double dx, dy;
	int shootCharge;
	double shootChargeProcent;
	bool passCharge;
	int team;
	int delay;
	bool willBeDelay;
	bool wasGoal;

public:
	Ball();
	Ball(SDL_Renderer* renderer, SDL_Point* position, State state = FREE_STATE);
	~Ball();
	int getState(void);
	void setCurrentPlayer(Player* p);
	Player* getCurrentPlayer(void);
	bool hasDelay(void);
	void setState(State state);
	void setPassCharge(bool charge);
	void setShootCharge(int charge);
	void setNextPlayer(Player* nextPlayer);
	void setDX(double dx);
	void setDY(double dy);
	void draw(SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL);
};

double distance(SDL_Point a, SDL_Point b);
Player* getClosestPlayer(SDL_Point* position, double angle, Object** obj, int team);