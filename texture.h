#include <string>
#include <SDL.h>

#pragma once

class Texture
{
private:
	SDL_Texture* tex;
	SDL_Renderer* rend;
	double x, y;
	double angle;
	int h, w;

public:
	Texture();
	Texture(std::string path, SDL_Renderer* r, int x = 0, int y = 0, double angle = 0);
	Texture(char t, std::string text, SDL_Renderer* r, int x = 0, int y = 0);
	~Texture();
	
	void freeTexture(void);
	void loadTexture(std::string path, SDL_Renderer* r, int x = 0, int y = 0);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setDimensions(double x, double y, int w = 0, int h = 0);
	int getHeight(void);
	int getWidth(void);
	void setAngle(double angle);
	double getAngle(void);
	SDL_Renderer* getRenderer(void);
	void setAlpha(Uint8 alpha);
	void renderTexture(SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
};