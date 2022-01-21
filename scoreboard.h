#pragma once

#include "object.h"

class scoreTime
{
private:
	int minutes;
	int seconds;
public:
	scoreTime();
	scoreTime(int seconds);
	~scoreTime();

	void addSeconds(int seconds = 1);
	void subSeconds(int seconds = 1);
	bool isZero(void);

	int getMinutes(void);
	int getSeconds(void);
};

class Scoreboard : public Object
{
private:
	int team1Score;
	int team2Score;
	int frameNumber;
	scoreTime* time;
	std::string team1Name;
	std::string team2Name;
	Texture* sTex;

public:
	Scoreboard();
	Scoreboard(SDL_Renderer* r, const char* team1, const char* team2, int time);
	~Scoreboard();

	void draw(SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL);

};