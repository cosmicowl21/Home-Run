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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Player.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("Player-animations.plist");

	player_sprite = Sprite::createWithSpriteFrameName("idle");
	player_sprite->setScale(SCALE_FACTOR);
	player_sprite->setFlippedX(true);

	Point point = Point(10, 2);
	Size size = player_sprite->getContentSize();


	player_sprite->setPosition(level->positionForTileCoordinate(size, point));

	player = new Player();
	player->retain();
	player->state = Player::State::Standing;

	Point origin = Director::getInstance()->getVisibleOrigin();
	Size wsize = Director::getInstance()->getVisibleSize();  //default screen size (or design resolution size, if you are using design resolution)
	Point *center = new Point(wsize.width / 2 + origin.x, wsize.height / 2 + origin.y);

	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(player_sprite->getPosition().x); // set to players x
	cameraTarget->setPositionY(wsize.height / 2 + origin.y); // center of height

	cameraTarget->retain();

	this->setupAnimations();
	this->addChild(player_sprite);
	this->schedule(schedule_selector(GameScene::updateScene));
	this->addChild(cameraTarget);

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();

	this->runAction(camera);
	return true;
}

void GameScene::setupAnimations() 
{

	AnimationCache *cache = AnimationCache::getInstance();
	Animation *animation = cache->getAnimation("walk");
	Animate* animate = Animate::create(animation);
	animate->getAnimation()->setRestoreOriginalFrame(true);
	animate->setDuration(0.80f);
	animate->setTarget(player_sprite);

	this->walkRight = animate;
	this->walkRight->retain();

}

void GameScene::updateScene(float delta) 
{

	cameraTarget->setPositionX(player_sprite->getPosition().x);
	this->updatePlayer(delta);

}

void GameScene::updatePlayer(float delta)
{

	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()) 
	{

		player->velocity.x = PLAYER_MAX_VELOCITY;

		if (player->grounded) 
		{
			player->state = Player::State::Walking;
		}

		player->facingRight = true;
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()) 
	{
		player->velocity.x = -PLAYER_MAX_VELOCITY;

		if (player->grounded)
		{
			player->state = Player::State::Walking;
		}
		player->facingRight = false;
	}
	// clamp the velocity to the maximum, x-axis only
	if (std::abs(player->velocity.x) > PLAYER_MAX_VELOCITY)
	{
		player->velocity.x = signum(player->velocity.x) * PLAYER_MAX_VELOCITY;
	}
	// clamp the velocity to 0 if it's < 1, and set the state to standing
	if (std::abs(player->velocity.x) < 1) 
	{
		player->velocity.x = 0;
		if (player->grounded) 
		{
			player->state = Player::State::Standing;
		}
	}

	// unscale the velocity by the inverse delta time and set
	// the latest position
	player->velocity = player->velocity * delta;
	player->position = player->position + player->velocity;
	player->velocity = player->velocity * 1 / delta;

	player->velocity.x *= DAMPING;
	this->updatePlayerSprite(delta);
}

void GameScene::updatePlayerSprite(float delta)
{

	if (player->state == Player::State::Walking)
	{

		if (walkRight->isDone()) 
		{
			walkRight->startWithTarget(player_sprite);
		}

		walkRight->step(delta);

		if (player->facingRight) 
		{
			player_sprite->setFlippedX(true);
		}
		else
		{
			player_sprite->setFlippedX(false);
		}

	}

	else if (player->state == Player::State::Jumping) 
	{

	}
	else 
	{
		player_sprite->setSpriteFrame(Sprite::createWithSpriteFrameName("idle")->getSpriteFrame());
	}

	player_sprite->setPositionX(player_sprite->getPositionX() + player->velocity.x);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end())
	{
		heldKeys.push_back(keyCode);
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

int GameScene::signum(float x)
{
	if (x > 0.0L)
		return 1.0L;
	else if (x < 0.0L)
		return -1.0L;
	else
		return 0.0L;
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
	setKeyboardEnabled(true);
}
GameScene::~GameScene(void)
{
}