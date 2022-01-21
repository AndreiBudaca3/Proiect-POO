#include <SDL.h>

#include "player.h"
#include "ObjectTypes.h"

#pragma once

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct matchHistory
{
	std::string player1Name;
	std::string player2Name;
	int player1Score;
	int player2Score;
};

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* wRenderer;

	Texture* backgorund;
	Texture* leftTextRows[10];
	Texture* rightTextRows[10];
	Object* objVec[OBJ_COUNT];
	bool flag[FLAG_NUMBER];

	bool quitGame;

	int player1color;
	int player2color;
	std::string player1Name;
	std::string player2Name;
	int player1Diffculty;
	int player2Diffculty;

	int matchLength;

	std::string hist[10];
	int nrHist;

public:

	Game(void);
	~Game(void);

	bool closeGame(void);
	void setGameStatus(bool state);
	void renderBackground(void);
	SDL_Renderer* getRenderer(void);
	Object** getObjectVector(void);
	bool* getFlagVector(void);

	void loadRightTexture(void);
	void loadLeftTexture(void);

	void startGame(void);
	void endGame(void);

	void increasePlayer1Difficulty(void);
	void increasePlayer2Difficulty(void);

	void clearScreen(void);
	void updateVisuals(void);
};
