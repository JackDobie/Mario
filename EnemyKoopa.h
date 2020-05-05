#pragma once

#ifndef _ENEMY_KOOPA_H

#include "Character.h"

class EnemyKoopa : public Character
{
protected:
	SDL_Renderer*	mRenderer;
	Vector2D		mPosition;
	Texture2D*		mTexture;
	virtual void	MoveLeft(float deltaTime);
	virtual void	MoveRight(float deltaTime);
	virtual void	Gravity(float deltaTime);

	float	mCollisionRadius;
private:
	float	mSingleSpriteWidth;
	float	mSingleSpriteHeight;

	bool	mInjured;
	float	mInjuredTime;
	void	FlipRightWayUp();
	
	bool	isAlive;
	bool	isDying;

	bool	hasBeenHit;

	FACING	mFacingDirection;
	bool	mMovingLeft, mMovingRight;
	float	mMovementSpeed;
	bool	mJumping;
	float	mJumpForce;
	bool	mCanJump;

	LevelMap*	mCurrentLevelMap;
public:
	EnemyKoopa();
	EnemyKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing, float movementSpeed, SoundEffect* sounds);
	~EnemyKoopa();
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

#endif // !_ENEMY_KOOPA_H