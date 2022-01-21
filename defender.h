#pragma once

#include "player.h"

class Defender : public Player
{
private:

	int verticalMovement;
	int horizontalMovement;
	double speed;
	bool isMarking;

	SDL_Point* pointToGo;
	SDL_Point* initialPosition;

public:
	Defender();
	Defender(SDL_Renderer* renderer, int teamColor, SDL_Point* position, int team, int difficulty, double angle = 0, bool selected = false);
	~Defender();

	void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL);
};