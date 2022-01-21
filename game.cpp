#include <iostream>
#include <fstream>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "game.h"
#include "ball.h"
#include "defender.h"
#include "striker.h"
#include "button.h"
#include "scoreboard.h"

Game::Game(void)
{
	int imgFlags = IMG_INIT_PNG;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Init error!\n" << SDL_GetError() << '\n';
		exit(-1);
	}
	else
	{
		std::cout << "SDL has initialized succesfully!\n";
	}

	window = SDL_CreateWindow("Football Mania", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == nullptr)
	{
		std::cout << "Error creating window!\n";
		exit(-1);
	}
	else
	{
		std::cout << "Window was created succesfully!\n";
	}

	wRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (wRenderer == nullptr)
	{
		std::cout << "Error creating renderer!\n";
		exit(-1);
	}
	else
	{
		std::cout << "Renderer was created succesfully!\n";
	}
	SDL_SetRenderDrawColor(wRenderer, 0x00, 0x00, 0x00, 0xFF);

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		exit(-1);
	}

	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		exit(-1);
	}

	quitGame = false;
	for (int i = 0; i < OBJ_COUNT; ++i)
		objVec[i] = nullptr;

	try
	{
		backgorund = new Texture(std::string("testAssets/terrain.png"), wRenderer);

		objVec[PLAY_BUTTON] = new Button(wRenderer, std::string("testAssets/start.png"), new SDL_Point{ 586, 180 }, 108, 48, MENU_FLAG, false);
		objVec[RESUME_BUTTON] = new Button(wRenderer, std::string("testAssets/resume.png"), new SDL_Point{ 586, 275 }, 108, 48, PAUSE_FLAG, false);
		objVec[CUSTOMISE_BUTTON] = new Button(wRenderer, std::string("testAssets/hist.png"), new SDL_Point{ 586, 370 }, 108, 48, RIGHT_TEXTURE_FLAG);
		objVec[QUIT_BUTTON] = new Button(wRenderer, std::string("testAssets/quit.png"), new SDL_Point{ 586, 465 }, 108, 48, QUIT_FLAG);
		objVec[CONTROLS_BUTTON] = new Button(wRenderer, std::string("testAssets/controls.png"), new SDL_Point{ 586, 275 }, 108, 48, LEFT_TEXTURE_FLAG);
		objVec[MENU_BUTTON] = new Button(wRenderer, std::string("testAssets/menu.png"), new SDL_Point{ 586, 370 }, 108, 40, MENU_FLAG);
	}
	catch (std::string err)
	{
		clearScreen();
		std::cout << err << '\n';

		SDL_Rect b{ 0, 0, 1280, 720 };
		SDL_SetRenderDrawColor(wRenderer, 0x01, 0x01, 0x01, 0xFF);
		SDL_RenderFillRect(wRenderer, &b);

		Texture error('t', err, wRenderer);
		error.renderTexture();
		updateVisuals();
		SDL_Delay(1000);

		quitGame = true;
	}

	for (int i = 0; i < FLAG_NUMBER; ++i)
		flag[i] = false;

	flag[PAUSE_FLAG] = false;
	flag[MENU_FLAG] = true;

	std::ifstream fin("config.in");

	char buffer[30];
	fin >> buffer >> player1color;
	fin >> buffer >> player2color;
	fin >> buffer >> player1Name;
	fin >> buffer >> player2Name;
	fin >> buffer >> matchLength;
	player1Diffculty = 0;
	player2Diffculty = 0;

	nrHist = 0;
	bool overflow = false;

	while (fin.getline(buffer, 256))
	{
		hist[nrHist++] = std::string(buffer);
		if (nrHist == 10)
		{
			nrHist = 0;
			overflow = true;
		}
	}

	if (overflow) nrHist = 10;

	fin.close();
}

void Game::setGameStatus(bool state)
{
	quitGame = !state;
}

Game::~Game(void)
{
	for (int i = 0; i < OBJ_COUNT; ++i)
	{
		if (objVec[i])
		{
			delete objVec[i];
			objVec[i] = nullptr;
		}
	}

	delete backgorund;
	
	for (int i = 0; i < 9; ++i)
	{
		if (rightTextRows[i])
			delete rightTextRows[i];
		if (leftTextRows[i])
			delete leftTextRows[i];
	}

	SDL_DestroyRenderer(wRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Renderer* Game::getRenderer(void)
{
	return wRenderer;
}

void Game::updateVisuals(void)
{
	SDL_RenderPresent(wRenderer);
}

void Game::clearScreen(void)
{
	SDL_RenderClear(wRenderer);
}

Object** Game::getObjectVector(void)
{
	return objVec;
}

void Game::renderBackground(void)
{
	backgorund->renderTexture();
}

bool* Game::getFlagVector(void)
{
	return flag;
}

void Game::startGame(void)
{
	try
	{
		objVec[BALL] = new Ball(wRenderer, new SDL_Point{ 630, 345 });

		objVec[TEAM1_PLAYER1] = new Goalkeeper(wRenderer, new SDL_Point{ 50, 340 }, 1);
		objVec[TEAM1_PLAYER2] = new Defender(wRenderer, player1color, new SDL_Point{ 265, 145 }, 1, player1Diffculty);
		objVec[TEAM1_PLAYER3] = new Defender(wRenderer, player1color, new SDL_Point{ 265, 530 }, 1, player1Diffculty);
		objVec[TEAM1_PLAYER4] = new Striker(wRenderer, player1color, new SDL_Point{ 620, 285 }, 1, player1Diffculty, 90, true);
		objVec[TEAM1_PLAYER5] = new Striker(wRenderer, player1color, new SDL_Point{ 620, 380 }, 1, player1Diffculty, -90);
		objVec[TEAM2_PLAYER1] = new Goalkeeper(wRenderer, new SDL_Point{ 1200, 340 }, 2, EASY, -180);
		objVec[TEAM2_PLAYER2] = new Striker(wRenderer, player2color, new SDL_Point{ 725, 200 }, 2, player2Diffculty, -180, true);
		objVec[TEAM2_PLAYER3] = new Striker(wRenderer, player2color, new SDL_Point{ 725, 480 }, 2, player2Diffculty, -180);
		objVec[TEAM2_PLAYER4] = new Defender(wRenderer, player2color, new SDL_Point{ 1000, 145 }, 2, player2Diffculty, -180);
		objVec[TEAM2_PLAYER5] = new Defender(wRenderer, player2color, new SDL_Point{ 1000, 530 }, 2, player2Diffculty, -180);

		objVec[SCOREBOARD] = new Scoreboard(wRenderer, player1Name.c_str(), player2Name.c_str(), matchLength);
	}
	catch(std::string err)
	{
		clearScreen();
		std::cout << err << '\n';

		SDL_Rect b{ 0, 0, 1280, 720 };
		SDL_SetRenderDrawColor(wRenderer, 0x01, 0x01, 0x01, 0xFF);
		SDL_RenderFillRect(wRenderer, &b);

		Texture error('t', err, wRenderer);
		error.renderTexture();
		updateVisuals();
		SDL_Delay(1000);

		throw(true);
		quitGame = true;

		flag[MENU_FLAG] = true;
	}
}

void Game::endGame(void)
{
	for (int i = BALL; i <= SCOREBOARD; ++i)
	{
		if (objVec[i])
		{
			delete objVec[i];
			objVec[i] = nullptr;
		}
	}
}

void Game::loadRightTexture(void)
{
	for (int i = 0; i < 10; ++i)
		if (rightTextRows[i])
		{
			delete rightTextRows[i];
			rightTextRows[i] = nullptr;
		}

	SDL_Rect rRect = { 150, 50, 400, 660 };
	SDL_SetRenderDrawColor(wRenderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(wRenderer, &rRect);

	for (int i = 0; i < nrHist; ++i)
	{
		rightTextRows[i] = new Texture('t', hist[i], wRenderer, 210, 50 + 60 * i);
		rightTextRows[i]->renderTexture();
	}
	
}

void Game::increasePlayer1Difficulty(void)
{
	++player1Diffculty;

	if (player1Diffculty > 3) player1Diffculty = 3;
}

void Game::increasePlayer2Difficulty(void)
{
	++player2Diffculty;

	if (player2Diffculty > 3) player2Diffculty = 3;
}

void Game::loadLeftTexture(void)
{
	for (int i = 0; i < 10; ++i)
		if (rightTextRows[i])
		{
			delete rightTextRows[i];
			rightTextRows[i] = nullptr;
		}

	SDL_Rect rRect = { 700, 50, 570, 660 };
	SDL_SetRenderDrawColor(wRenderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(wRenderer, &rRect);

	rightTextRows[0] = new Texture('t', std::string("Player2"), wRenderer, 705, 50);
	rightTextRows[1] = new Texture('t', std::string("Move: UP/DOWN/LEFT/RIGHT"), wRenderer, 700, 140);
	rightTextRows[2] = new Texture('t', std::string("Pass: NUM2"), wRenderer, 705, 200);
	rightTextRows[3] = new Texture('t', std::string("Shoot: NUM3"), wRenderer, 705, 260);
	rightTextRows[4] = new Texture('t', std::string("Switch: NUM_ENTER"), wRenderer, 705, 320);
	rightTextRows[5] = new Texture('t', std::string("Player1"), wRenderer, 705, 390);
	rightTextRows[6] = new Texture('t', std::string("Move: W/A/S/D"), wRenderer, 705, 470);
	rightTextRows[7] = new Texture('t', std::string("Pass: K"), wRenderer, 705, 530);
	rightTextRows[8] = new Texture('t', std::string("Shoot: L"), wRenderer, 705, 590);
	rightTextRows[9] = new Texture('t', std::string("Switch: E"), wRenderer, 705, 650);


	for (int i = 0; i < 10; ++i)
	{
		rightTextRows[i]->renderTexture();
	}
}

bool Game::closeGame(void)
{
	return quitGame;
}