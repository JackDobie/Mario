#pragma once

#ifndef _ENEMY_CRAB_H

#include "Character.h"

class EnemyCrab : public Character
{
protected:
	SDL_Renderer*	mRenderer;
	Vector2D		mPosition;
	Texture2D*		mTexture;
	virtual void	MoveLeft(float deltaTime);
	virtual void	MoveRight(float deltaTime);
	virtual void	Gravity(float deltaTime);
private:
	float	mSingleSpriteWidth;
	float	mSingleSpriteHeight;

	bool	mInjured;
	float	mInjuredTime;
	void	FlipRightWayUp();

	bool	isAlive;
	bool	isDying;

	bool	hasBeenHit;
	int		hitCount;

	FACING	mFacingDirection;
	bool	mMovingLeft, mMovingRight;
	float	mMovementSpeed;
	bool	mJumping;
	float	mJumpForce;
	bool	mCanJump;

	LevelMap* mCurrentLevelMap;

	float	animCounter;
public:
	EnemyCrab();
	EnemyCrab(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing, float movementSpeed, SoundEffect* sounds);
	~EnemyCrab();
	void	TakeDamage();
	void	Jump();

	virtual	void Render();
	virtual	void Update(float deltaTime, SDL_Event e);

	float	GetCollisionRadius()
	{
		return mCollisionRadius;
	};
	Rect2D	GetCollisionBox();

	void	CheckIfHit(int h, int w);

	void	Turn();

	Vector2D GetCentralPosition();

	bool	IsInPipe();
	bool	GetInjured();
	bool	GetAlive();
	void	Kill();
	bool	HasBeenHit();
};

#endif // !_ENEMY_CRAB_H