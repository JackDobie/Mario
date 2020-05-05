#pragma once

#ifndef _LEVELMAP_H
#define _LEVELMAP_H

#include "Constants.h"

class LevelMap
{
public:
	LevelMap();
	LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	~LevelMap();

	int GetTileAt(unsigned int h, unsigned int w);
	void ChangeTileAt(int h, int w, int changeTo);
private:
	int** mMap;
};

#endif // !_LEVELMAP_H