#include <iostream>

#include "button.h"
#include "game.h"
#include "ball.h"

int main( int argc, char* args[] )
{
	Game* game = new Game();
	SDL_Event e[100];
	int eventCounter = 0;

	Object** obj = game->getObjectVector();
	bool* flag = game->getFlagVector();

	while (!game->closeGame())
	{
		game->clearScreen();
		game->renderBackground();

		if (flag[QUIT_FLAG])
			game->setGameStatus(false);

		while (SDL_PollEvent(e + eventCounter))
		{
			++eventCounter;
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (flag[MENU_FLAG])
		{
			if (flag[GAME_LOADED])
			{
				flag[GAME_LOADED] = false;
				flag[PAUSE_FLAG] = false;
				flag[END_MATCH_FLAG] = false;
				game->endGame();

				if (flag[TEAM1_WIN_FLAG])
				{
					game->increasePlayer1Difficulty();
					flag[TEAM1_WIN_FLAG] = false;
				}
				if (flag[TEAM2_WIN_FLAG])
				{
					game->increasePlayer2Difficulty();
					flag[TEAM2_WIN_FLAG] = false;
				}
			}

			if (flag[RIGHT_TEXTURE_FLAG])
			{
				game->loadRightTexture();
			}

			if (flag[LEFT_TEXTURE_FLAG])
			{
				game->loadLeftTexture();
			}

			for (int i = 0; i < eventCounter; ++i)
			{
				if (e[i].type == SDL_QUIT)
					game->setGameStatus(false);

				for (int j = PLAY_BUTTON; j <= CUSTOMISE_BUTTON; ++j)
					if (obj[j] != nullptr)
						obj[j]->update(e + i, currentKeyStates, obj, flag);
			}

			for (int j = PLAY_BUTTON; j <= CUSTOMISE_BUTTON; ++j)
				if (obj[j] != nullptr)
					obj[j]->draw();
		}
		else
		{
			if (flag[END_MATCH_FLAG] == false)
			{

				flag[RIGHT_TEXTURE_FLAG] = false;
				flag[LEFT_TEXTURE_FLAG] = false;

				if (flag[GAME_LOADED] == false)
				{
					flag[GAME_LOADED] = true;
					try
					{
						game->startGame();
					}
					catch (bool needToBreak)
					{
						if (needToBreak)
							break;
					}
				}

				for (int i = 0; i < eventCounter; ++i)
				{
					if (e[i].type == SDL_QUIT)
						game->setGameStatus(false);

					if (flag[PAUSE_FLAG] == true)
					{
						for (int j = RESUME_BUTTON; j <= MENU_BUTTON; ++j)
							if (obj[j] != nullptr)
								obj[j]->update(e + i, currentKeyStates, obj, flag);
					}
				}

				if (flag[PAUSE_FLAG] == false)
				{
					for (int i = BALL; i <= SCOREBOARD; ++i)
						if (obj[i] != nullptr)
							obj[i]->update(NULL, currentKeyStates, obj, flag);

					for (int i = BALL; i <= SCOREBOARD; ++i)
						if (obj[i] != nullptr)
							obj[i]->draw();
				}
				else
				{
					for (int i = RESUME_BUTTON; i <= MENU_BUTTON; ++i)
						if (obj[i] != nullptr)
							obj[i]->draw();
				}
			}
			else
			{
				flag[MENU_FLAG] = true;
			}
		}

		eventCounter = 0;
		game->updateVisuals();
	}

	game->endGame();

	delete game;

	return 0;
}