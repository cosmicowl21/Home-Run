#include "GameScene.h"

USING_NS_CC;

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

	this->addChild(level->getMap());

	return true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
	level->release();
}