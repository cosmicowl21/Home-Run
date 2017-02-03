#include "Level.h"

void Level::loadMap(const char* mapname) {

	map = TMXTiledMap::create(mapname);
	map->retain();

}

TMXTiledMap * Level::getMap() {
	return map;
}

Level::Level(void)
{
}

Level::~Level(void)
{
	map->release();
}