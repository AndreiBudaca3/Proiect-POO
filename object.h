#include "texture.h"

#pragma once

class Object
{
private:
	SDL_Point* position;
	int w, h;

public:
	Object();
	Object(SDL_Point* position, int width, int height);
	virtual ~Object();

	SDL_Point* getPosition(void);
	void setPosition(int x, int y);
	int getWidth(void);
	void setWidth(int w);
	int getHeight(void);
	void setHeight(int h);

	virtual void draw(SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) = 0;
	virtual void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL) = 0;
};

void resetPosition(Object** objVec, int team, bool goal = false, bool out = false);