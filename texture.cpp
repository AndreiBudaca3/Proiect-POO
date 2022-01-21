#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

#include "texture.h"

Texture::Texture()
{
	tex = nullptr;
	x = y = w = h = 0;
	angle = 0;
	rend = nullptr;
}

Texture::Texture(std::string path, SDL_Renderer* rend, int x, int y, double angle)
{
	SDL_Surface* auxSurface = nullptr;
	
	auxSurface = IMG_Load(path.c_str());
	if (auxSurface == NULL)
	{
		throw(std::string("Unable to load image ") + path);
		return;
	}

	w = auxSurface->w;
	h = auxSurface->h;
	tex = SDL_CreateTextureFromSurface(rend, auxSurface);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	this->rend = rend;
	this->x = x;
	this->y = y;
	this->angle = angle;

	SDL_FreeSurface(auxSurface);
}

Texture::Texture(char t, std::string text, SDL_Renderer* r, int x, int y)
{
	TTF_Font* gFont = TTF_OpenFont("testAssets/impact.ttf", 50);
	if (gFont == NULL)
	{
		throw(std::string("Failed to load font!"));
		return;
	}
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), SDL_Color{0xFF, 0xFF, 0xFF, 0xFF});
	if (textSurface == NULL)
	{
		//throw(std::string("Unable to render text surface!"));
		return;
	}
	else
	{
		//Create texture from surface pixels
		tex = SDL_CreateTextureFromSurface(r, textSurface);
		if (tex == NULL)
		{
			throw(std::string("Unable to create texture from rendered text!"));
			return;
		}
		else
		{
			w = textSurface->w;
			h = textSurface->h;
		}

		this->rend = r;
		this->x = x;
		this->y = y;

		TTF_CloseFont(gFont);
		SDL_FreeSurface(textSurface);
	}
}

Texture::~Texture()
{
	freeTexture();
}

void Texture::freeTexture(void)
{
	if (tex != nullptr)
	{
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}

	rend = nullptr;
}

void Texture::loadTexture(std::string path, SDL_Renderer* rend, int x, int y)
{
	SDL_Surface* auxSurface = nullptr;

	freeTexture();

	auxSurface = IMG_Load(path.c_str());
	if (auxSurface == NULL)
	{
		std::cout << "Unable to load image " << path.c_str() << "! \nSDL_image Error : " << IMG_GetError() << '\n';
		exit(-1);
	}

	w = auxSurface->w;
	h = auxSurface->h;
	tex = SDL_CreateTextureFromSurface(rend, auxSurface);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	this->rend = rend;
	this->x = x;
	this->y = y;

	SDL_FreeSurface(auxSurface);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(tex, red, green, blue);
}

void Texture::setDimensions(double x, double y, int w, int h)
{
	this->x = x;
	this->y = y;
	if(w != 0)
		this->w = w;
	if(h != 0)
		this->h = h;
}

int Texture::getHeight(void)
{
	return h;
}

int Texture::getWidth(void)
{
	return w;
}

void Texture::setAngle(double angle)
{
	this->angle = angle;
}

double Texture::getAngle(void)
{
	return angle;
}

SDL_Renderer* Texture::getRenderer(void)
{
	return rend;
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(tex, alpha);
}

void Texture::renderTexture(SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect texRect = { (int)x, (int)y, w, h };

	if (clip != NULL)
	{
		texRect.w = clip->w;
		texRect.h = clip->h;
	}

	SDL_RenderCopyEx(rend, tex, clip, &texRect, angle, center, flip);
}

