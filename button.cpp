#include <iostream>

#include "button.h"

int mouseOffDelay = 0;

Button::Button() : Object()
{
	try
	{
		texture = new Texture();
	}
	catch (std::string err)
	{
		throw (err);
	}
	state = BUTTON_MOUSE_OFF;
	lastState = BUTTON_MOUSE_OFF;
	x = y = w = h = 0;
}

Button::Button(SDL_Renderer* renderer, std::string texturePath, SDL_Point* position, int width, int height, int flagToChange, bool flagValue) :
	Object(position, width, height)
{
	state = BUTTON_MOUSE_OFF;
	lastState = BUTTON_MOUSE_OFF;

	try
	{
	texture = new Texture(texturePath, renderer, position->x, position->y);
	}
	catch (std::string err)
	{
		throw (err);
	}

	x = position->x;
	y = position->y;
	w = width;
	h = height;
	this->flagToChange = flagToChange;
	this->flagValue = flagValue;
}

Button::~Button()
{
	state = BUTTON_MOUSE_OFF;

	if (texture != nullptr)
		delete texture;
	texture = nullptr;
}

void Button::update(SDL_Event* e, const Uint8* keyboardState, Object** obj, bool* flag)
{
	--mouseOffDelay;

	int x, y;
	bool isInside = false;
	SDL_Point* position = getPosition();
	int w = getWidth();
	int h = getHeight();

	if (e != NULL)
	{
		if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
		{

			SDL_GetMouseState(&x, &y);

			if (x > position->x && x < position->x + w && y > position->y && y < position->y + h)
				isInside = true;

			if (lastState == BUTTON_MOUSE_CLICKDOWN)
			{
				if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
					state = BUTTON_MOUSE_CLICKUP;
			}
			else
			{
				if (isInside)
				{
					if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
						state = BUTTON_MOUSE_CLICKDOWN;
					else if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
						state = BUTTON_MOUSE_CLICKUP;
					else
						state = BUTTON_MOUSE_OVER;
				}
				else
				{
					state = BUTTON_MOUSE_OFF;
				}
			}

			if (state != lastState)
			{
				switch (state)
				{
				case BUTTON_MOUSE_OFF:
					if (lastState != BUTTON_MOUSE_CLICKUP)
					{
						texture->setColor(0xFF, 0xFF, 0xFF);
						texture->setDimensions(position->x, position->y, w, h);
					}
					break;
				case BUTTON_MOUSE_OVER:
					texture->setColor(0xAF, 0xAF, 0xAF);
					if (lastState == BUTTON_MOUSE_OFF)
					{
						texture->setDimensions(position->x - 2, position->y - 2, w + 4, h + 4);
					}
					break;
				case BUTTON_MOUSE_CLICKDOWN:
					texture->setDimensions(position->x + 2, position->y + 2, w - 4, h - 4);
					break;
				case BUTTON_MOUSE_CLICKUP:
					if (!isInside)
					{
						texture->setColor(0xFF, 0xFF, 0xFF);
						texture->setDimensions(position->x, position->y, w, h);
					}
					else
					{
						texture->setDimensions(position->x - 2, position->y - 2, w + 4, h + 4);

						if (mouseOffDelay <= 0)
						{
							std::cout << "flag to change: " << flagToChange << "\t flag value: " << flagValue << '\n';
							flag[flagToChange] = flagValue;
							mouseOffDelay = 5;
						}
					}
					break;
				default:
					break;
				}
				lastState = state;
			}
		}
	}
}

void Button::draw(SDL_Point* center, SDL_RendererFlip flip)
{
	texture->renderTexture(NULL, center, flip);
}