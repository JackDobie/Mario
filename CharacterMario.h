#pragma once

#ifndef _CHARACTER_MARIO_H

#include "Character.h"

class CharacterMario : public Character
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void Gravity(float deltaTime);

	float	mCollisionRadius;
private:
	FACING mFacingDirection;
	bool mMovingLeft, mMovingRight;
	const float MovementSpeed = PLAYER_SPEED;//100;
	bool mJumping;
	bool mCanJump;
	float mJumpForce = 100;
	LevelMap* mCurrentLevelMap;
	bool isAlive;
	bool endOfGame;
	int livesLeft;
	Vector2D respawnPosition;
	bool isRespawning;
	float respawnTimer;
	SoundEffect* mSounds;

	void Jump()
	{
		if (!mJumping && mCanJump) //if not already jumping, reset values
		{
			mJumpForce = INITIAL_JUMP_FORCE;
			mJumping = true;
			mCanJump = false; //stops multiple jumps
		}
	};
public:
	CharacterMario();
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map, SoundEffect* sounds);
	~CharacterMario();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition();
	Rect2D GetCollisionBox();
	bool IsJumping()
	{
		return mJumping;
	};
	void CancelJump()
	{
		mJumpForce = 0.0f;
	};

	float	GetCollisionRadius()
	{
		return mCollisionRadius;
	};
	bool GetRespawning();
	bool GetAlive();
	int GetLivesLeft();
	void Kill();
	bool GetEndOfGame()
	{
		return endOfGame;
	};
};

#endif // !_CHARACTER_MARIO_H