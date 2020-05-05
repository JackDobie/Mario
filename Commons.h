#pragma once

struct Vector2D
{
	float x, y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float InitialX, float InitialY)
	{
		x = InitialX;
		y = InitialY;
	}
};

struct Rect2D
{
	float x, y, width, height;

	Rect2D()
	{
		x = 0.0f;
		y = 0.0f;
		width = 0.0f;
		height = 0.0f;
	}

	Rect2D(float posX, float posY, float rectWidth, float rectHeight)
	{
		x = posX;
		y = posY;
		width = rectWidth;
		height = rectHeight;
	}
};

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT
};