#include "object.h"

#pragma once

enum ButtonState
{
	BUTTON_MOUSE_OFF,
	BUTTON_MOUSE_OVER,
	BUTTON_MOUSE_CLICKDOWN,
	BUTTON_MOUSE_CLICKUP,
	BUTTON_FUNCTION_NUMBER
};


class Button : public Object
{
private:
	ButtonState state, lastState;
	Texture* texture;
	int x, y, w, h;

	int flagToChange;
	bool flagValue;

public:
	Button();
	Button(SDL_Renderer* renderer, std::string texturePath, SDL_Point* position, int width, int height, int flagToChange, bool flagValue = true);
	~Button();

	void update(SDL_Event* e = NULL, const Uint8* keyboardState = 0, Object** obj = NULL, bool* flag = NULL);
	void draw(SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
};
