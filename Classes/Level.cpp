#include "Level.h"
#include "Global.h"

void Level::loadMap(const char* mapname) {

	map = TMXTiledMap::create(mapname);
	map->retain();

}

TMXTiledMap * Level::getMap() {
	return map;
}

Point Level::positionForTileCoordinate(Size s, Point point) {

	float x = floor(s.width / 2 * SCALE_FACTOR + point.x * map->getTileSize().width * SCALE_FACTOR);
	float y = floor(s.height / 2 * SCALE_FACTOR + point.y * map->getTileSize().height * SCALE_FACTOR);

	return Point(x, y);

}

Level::Level(void)
{
}

Level::~Level(void)
{
	map->release();
}