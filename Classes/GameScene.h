#pragma once
#include <iostream>
#include "cocos2d.h"
#include "Level.h"
#include "Player.h"
#include <algorithm>
#include <vector>
#include <string>

using namespace std;
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:

	//FIELDS
	Level * level;
	Player *player;
	Sprite *player_sprite;
	Sprite *cameraTarget;

	Animate *walkRight;
	Animate *jumping;
	Animate *falling;

	Follow *camera;

	vector<EventKeyboard::KeyCode> heldKeys;

	//CONSTRUCTOR & METHODS

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void updateScene(float interval);
	void updatePlayer(float interval);
	void updatePlayerSprite(float interval);

	int signum(float x);

	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);
	void setupAnimations();

	CREATE_FUNC(GameScene);

	GameScene(void);
	virtual ~GameScene(void);
};