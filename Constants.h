#pragma once

#ifndef _CONSTANTS_H
//Screen dimensions
#define SCREEN_WIDTH			512
#define SCREEN_HEIGHT			416
//Map dimensions
#define MAP_HEIGHT				13
#define MAP_WIDTH				16
#define TILE_HEIGHT				32
#define TILE_WIDTH				32
//Player
#define PLAYER_SPEED			100.0f
#define JUMP_FORCE_DECREMENT	550.0f
#define INITIAL_JUMP_FORCE		550.0f
#define GRAVITY					200.0f
#define PLAYER_LIVES_MAX		2
//Level 1
#define L1_ENEMIES_MAX			10
#define L1_MAX_ONSCREEN_ENEMIES	5
//Enemy
#define KOOPA_SPEED				50.0f
#define CRAB_SPEED_NORMAL		50.0f
#define CRAB_SPEED_ANGRY		80.0f
#define INJURED_TIME			5.0f
#define INITIAL_JUMP_FORCE_SMALL 250.0f
//Screenshake
#define SCREENSHAKE_DURATION	0.25f
#endif // !_CONSTANTS_H