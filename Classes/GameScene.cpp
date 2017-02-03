#include "GameScene.h"
#include "Global.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	level = new Level();
	level->loadMap("level1.tmx");
	level->retain();


	auto director = Director::getInstance();
	level->getMap()->setScale(SCALE_FACTOR);

	this->addChild(level->getMap());

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("solbrain.plist");

	player_sprite = Sprite::createWithSpriteFrameName("idle");
	player_sprite->setScale(SCALE_FACTOR);
	player_sprite->setFlippedX(true);

	Point point = Point(10, 2);
	Size size = player_sprite->getContentSize();

	player_sprite->setPosition(level->positionForTileCoordinate(size, point));

	player = new Player();
	player->retain();

	this->addChild(player_sprite);

	return true;
}

GameScene::GameScene() {
}

GameScene::~GameScene(void)
{
}