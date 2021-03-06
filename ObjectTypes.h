#pragma once

enum ObjectTypes
{
	BALL,

	TEAM1_PLAYER1,
	TEAM1_PLAYER2,
	TEAM1_PLAYER3,
	TEAM1_PLAYER4,
	TEAM1_PLAYER5,

	TEAM2_PLAYER1,
	TEAM2_PLAYER2,
	TEAM2_PLAYER3,
	TEAM2_PLAYER4,
	TEAM2_PLAYER5,

	SCOREBOARD,

	PLAY_BUTTON,
	QUIT_BUTTON,
	CONTROLS_BUTTON,
	CUSTOMISE_BUTTON,

	RESUME_BUTTON,
	MENU_BUTTON,

	OBJ_COUNT,
};

enum State
{
	FREE_STATE,
	PLAYER_STATE,
	SHOOT_STATE,
	PASS_STATE
};

enum Difficulty
{
	EASY,
	MEDIUM,
	HARD
};

enum Flags
{
	QUIT_FLAG,
	TEAM1_GOAL_FLAG,
	TEAM2_GOAL_FLAG,
	END_MATCH_FLAG,
	TEAM1_WIN_FLAG,
	TEAM2_WIN_FLAG,
	PAUSE_FLAG,
	MENU_FLAG,
	IN_GAME_FLAG,
	GAME_LOADED,
	RIGHT_TEXTURE_FLAG,
	LEFT_TEXTURE_FLAG,

	FLAG_NUMBER
};

const double PI = 3.14159265359;