#pragma once

#include "player.h"

class Goalkeeper : public Player
{
private:
	int verticalMovement;
	int horizontalMovement;
	double dx, dy;
	double speed;

	SDL_Point* pointToGo;
	SDL_Point* initialPosition;

public:
	Goalkeeper();
	Goalkeeper(SDL_Renderer* renderer, SDL_Point* position, int team, double angle = 0, bool selected = false);
	~Goalkeeper();

	void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL);
};