#include <iostream>
#include <fstream>

#include "scoreboard.h"
#include "ObjectTypes.h"

scoreTime::scoreTime()
{
	minutes = 0;
	seconds = 0;
}

scoreTime::scoreTime(int seconds)
{
	this->seconds = seconds;
	this->minutes = this->seconds / 60;
	this->seconds %= 60;
}

scoreTime::~scoreTime()
{
	minutes = 0;
	seconds = 0;
}

void scoreTime::addSeconds(int seconds)
{
	this->seconds += seconds;
	this->minutes += this->seconds / 60;
	this->seconds %= 60;
}

void scoreTime::subSeconds(int seconds)
{
	this->seconds -= seconds;

	while (this->seconds < 0)
	{
		--this->minutes;
		this->seconds += 60;
	}
	
	if (this->minutes < 0)
	{
		this->minutes = 0;
		this->seconds = 0;
	}

}

bool scoreTime::isZero(void)
{
	return minutes == 0 && seconds == 0;
}

int scoreTime::getMinutes(void)
{
	return minutes;
}

int scoreTime::getSeconds(void)
{
	return seconds;
}

Scoreboard::Scoreboard() : Object()
{
	sTex = nullptr;
}

Scoreboard::Scoreboard(SDL_Renderer* r, const char* team1, const char* team2, int time) : Object(new SDL_Point{515, 2}, 250, 35)
{
	team1Score = 0;
	team2Score = 0;
	this->time = new scoreTime(time);
	team1Name = std::string(team1);
	team2Name = std::string(team2);

	std::string aux;
	char buffer1[4];
	char buffer2[4];
	char buffer3[3];
	char buffer4[3];

	_itoa_s(this->time->getMinutes(), buffer1, 10);
	_itoa_s(this->time->getSeconds(), buffer2, 10);
	_itoa_s(team1Score, buffer3, 10);
	_itoa_s(team2Score, buffer4, 10);

	aux = team1Name + std::string("   ") + std::string(buffer3) + std::string(" : ") + std::string(buffer4) + std::string("   ") + team2Name;
	aux = aux + std::string("     ") + std::string(buffer1) + std::string(" : ") + std::string(buffer2);

	try
	{
		sTex = new Texture('t', aux, r);
	}
	catch (std::string err)
	{
		throw (err);
	}
	sTex->setDimensions(520, 2, 240, 35);

	frameNumber = 60;
}

Scoreboard::~Scoreboard()
{
	delete time;
	delete sTex;
}

void Scoreboard::draw(SDL_Point* center, SDL_RendererFlip flip)
{
	std::string aux;
	char buffer1[4];
	char buffer2[4];
	char buffer3[3];
	char buffer4[3];

	_itoa_s(this->time->getMinutes(), buffer1, 10);
	_itoa_s(this->time->getSeconds(), buffer2, 10);
	_itoa_s(team1Score, buffer3, 10);
	_itoa_s(team2Score, buffer4, 10);

	aux = team1Name + std::string("   ") + std::string(buffer3) + std::string(" : ") + std::string(buffer4) + std::string("   ") + team2Name;
	aux = aux + std::string("     ") + std::string(buffer1) + std::string(" : ") + std::string(buffer2);

	SDL_Renderer* r = sTex->getRenderer();
	delete sTex;
	sTex = new Texture('t', aux, r);
	sTex->setDimensions(520, 2, 240, 35);

	SDL_Rect zone = { 515, 5, 250, 35 };
	SDL_SetRenderDrawColor(sTex->getRenderer(), 0X96, 0x4B, 0x00, 0xFF);
	SDL_RenderFillRect(sTex->getRenderer(), &zone);
	sTex->renderTexture();
}

void Scoreboard::update(SDL_Event* e, const Uint8* keyboardState, Object** obj, bool* flag)
{
	++frameNumber;

	if (frameNumber >= 60)
	{
		frameNumber = 0;
		time->subSeconds(1);
	}

	if (flag)
	{
		if (flag[TEAM1_GOAL_FLAG] == true)
		{
			++team1Score;
			if (team1Score > 99) team1Score = 99;

			flag[TEAM1_GOAL_FLAG] = false;
		}

		if (flag[TEAM2_GOAL_FLAG] == true)
		{
			++team2Score;
			if (team2Score > 99) team2Score = 99;
			flag[TEAM2_GOAL_FLAG] = false;
		}
	}

	if (time->isZero())
	{
		flag[END_MATCH_FLAG] = true;

		std::ofstream fout;
		fout.open("config.in", std::ios_base::app);

		fout << team1Name << ' ' << team1Score << '-' << team2Score << ' ' << team2Name << '\n';

		if (team1Score > team2Score) flag[TEAM1_WIN_FLAG] = true;
		if (team1Score < team2Score) flag[TEAM2_WIN_FLAG] = true;

		fout.close();
	}

}

