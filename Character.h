#pragma once

#ifndef _CHARACTER_H
#include "SDL.h"
#include "Commons.h"
#include "Texture2D.h"
#include "Constants.h"
#include "LevelMap.h"
#include "SoundEffect.h"
#include <iostream>
#include <string>

class Character
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void AddGravity(float deltaTime);

	float mCollisionRadius;
private:
	FACING mFacingDirection;
	bool mMovingLeft, mMovingRight;
	const float MovementSpeed = 100;
	bool mJumping;
	bool mCanJump;
	float mJumpForce = 100;
	//const float FloorPos = 330; //has gravity if char y is higher than this on screen
	//const float GravityForce = 150;
	LevelMap* mCurrentLevelMap;

	void Jump(float deltaTime)
	{
		if (!mJumping) //if not already jumping, reset values
		{
			mJumpForce = INITIAL_JUMP_FORCE;
			mJumping = true;
			mCanJump = false; //stops multiple jumps
		}
	};
public:
	Character();
	Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition();
	Vector2D GetCentralPosition();

	float GetCollisionRadius()
	{
		return mCollisionRadius;
	};
	Rect2D GetCollisionBox();

	bool IsJumping();
	void CancelJump()
	{
		mJumpForce = 0.0f;
	};
};
#endif // !_CHARACTER_H
