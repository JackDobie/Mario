#pragma once

#ifndef _CHARACTER_LUIGI_H

#include "Character.h"

class CharacterLuigi : public Character
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void Gravity(float deltaTime);

	float mCollisionRadius;
private:
	FACING mFacingDirection;
	bool mMovingLeft, mMovingRight;
	const float MovementSpeed = PLAYER_SPEED;//100;
	bool mJumping;
	bool mCanJump;
	float mJumpForce = 100;
	LevelMap* mCurrentLevelMap;
	bool isAlive;
	int livesLeft;
	Vector2D respawnPosition;
	bool isRespawning;
	float respawnTimer;

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
	CharacterLuigi();
	CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterLuigi();

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

	bool GetAlive();
	int GetLivesLeft();
	void Kill();
};

#endif // !_CHARACTER_LUIGI_H