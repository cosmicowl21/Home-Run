#include "MainMenuScene.h"
#include "GameScene.h"
//#include "Definitions.h"
//#include "TheOptionsScene.h"
#include "SimpleAudioEngine.h"
//#include "Quit.h"

using namespace CocosDenshion;

#define MENU_SFX "menu.mp3"//sound for the splash screen


USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();//init size
	Vec2 origin = Director::getInstance()->getVisibleOrigin();// init the origin 


	auto backgroundSprite = Sprite::create("HomeRun.png");// main menu image 
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 3 + origin.y));
	this->addChild(backgroundSprite);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_SFX);//menu sound 

	//button mapping to other screens

	//when click play it goes to the game scene
	auto NewGame = MenuItemImage::create("newGame.png", "newGamePushed.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
	NewGame->setPosition(Point(visibleSize.width / 2.7+ origin.x, visibleSize.height / 4 + origin.y));

	//options
	//auto options = MenuItemImage::create("options.png", "options.png", CC_CALLBACK_1(MainMenuScene::GoToTheOptionsScene, this));
	//options->setPosition(Point(visibleSize.width / 2.5 + origin.x, visibleSize.height / 5 + origin.y));

	auto closeItem = MenuItemImage::create("Quit.png", "QuitPushed.png", CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
	closeItem->setPosition(Point(visibleSize.width / 1.5 + origin.x, visibleSize.height / 4 + origin.y));


	auto NewGameButton = Menu::create(NewGame, NULL);
	NewGameButton->setPosition(Point::ZERO);
	this->addChild(NewGameButton);

	//auto optionsMenu = Menu::create(options, NULL);
	//optionsMenu->setPosition(Point::ZERO);
	//this->addChild(optionsMenu);

	auto exit = Menu::create(closeItem, NULL);
	exit->setPosition(Point::ZERO);
	this->addChild(exit);

	return true;
}

void MainMenuScene::GoToGameScene(Ref *sender)//go to game scene method implementation 
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(2.0, scene));
}

void MainMenuScene::GoToTheOptionsScene(Ref *sender)//go to game scene method implementation 
{
	//auto scene = TheOptionsScene::createScene();
	//Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}
