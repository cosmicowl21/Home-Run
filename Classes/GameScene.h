#pragma once

#include "cocos2d.h"
#include "Level.h"

class GameScene : public cocos2d::Layer
{
public:

	//FIELDS
	Level * level;

	//CONSTRUCTOR & METHODS
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(GameScene);

	GameScene(void);
	virtual ~GameScene(void);
};