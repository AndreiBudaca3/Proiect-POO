#include "player.h"

#pragma once


class Striker : public Player
{
private:
	int verticalMovement;
	int horizontalMovement;
	double speed;
	bool isMarking;

	SDL_Point* pointToGo;
	SDL_Point* initialPosition;
public:
	Striker();
	Striker(SDL_Renderer* renderer, int teamColor, SDL_Point* position, int team, int difficulty, double angle = 0, bool selected = false);
	~Striker();

	SDL_Point* getPointToGo(void);

	void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL);
};