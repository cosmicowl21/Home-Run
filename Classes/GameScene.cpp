
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
	AnimationCache::getInstance()->addAnimationsWithFile("solbrain-animations.plist");

	player_sprite = Sprite::createWithSpriteFrameName("idle");
	player_sprite->setScale(SCALE_FACTOR);
	player_sprite->setAnchorPoint(Point(0, 0));

	player_sprite->setFlippedX(true);

	Point point = Point(10, 2);

	player_sprite->setPosition(level->tileCoordinateToPosition(point));

	player = new Player();
	player->retain();
	player->state = Player::State::Standing;
	player->position = player_sprite->getPosition();
	player->player_size.width = player_sprite->getBoundingBox().size.width;
	player->player_size.height = player_sprite->getBoundingBox().size.height;

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

	rectWithBorder = DrawNode::create();
	Vec2 vertices[] =
	{
		Vec2(0, player_sprite->getBoundingBox().size.height),
		Vec2(player_sprite->getBoundingBox().size.width, player_sprite->getBoundingBox().size.height),
		Vec2(player_sprite->getBoundingBox().size.width, 0),
		Vec2(0,0)
	};

	rectWithBorder->drawPolygon(vertices, 4, Color4F(0.0f, 0.3f, 0.3f, 1), 0, Color4F(0.0f, 0.2f, 0.0f, 1));

	addChild(rectWithBorder);

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();

	loadEnemies();

	this->runAction(camera);
	return true;
}

void GameScene::setupAnimations() {

	AnimationCache *cache = AnimationCache::getInstance();
	Animation *animation = cache->getAnimation("walk");
	Animate* animate = Animate::create(animation);

	animate->getAnimation()->setRestoreOriginalFrame(true);
	animate->setDuration(0.80f);
	animate->setTarget(player_sprite);

	this->walkRight = animate;
	this->walkRight->retain();

}

void GameScene::loadEnemies() {

	Sprite *enemy1 = Sprite::create("mum.png");
	enemy1->setPosition(level->tileCoordinateToPosition(Point(33, 2)));
	enemy1->setAnchorPoint(Point::ZERO);
	enemy1->setScale(ENEMY_SCALE_FACTOR);
	enemy1->setFlippedX(true);
	enemy1->retain();

	enemyList.push_back(enemy1);
	this->addChild(enemy1);

	Sprite *enemy2 = Sprite::create("mum.png");
	enemy2->setPosition(level->tileCoordinateToPosition(Point(44, 2)));
	enemy2->setAnchorPoint(Point::ZERO);
	enemy2->setScale(ENEMY_SCALE_FACTOR);
	enemy2->setFlippedX(true);
	enemy2->retain();

	enemyList.push_back(enemy2);
	this->addChild(enemy2);


	Sprite *enemy3 = Sprite::create("mum.png");
	enemy3->setPosition(level->tileCoordinateToPosition(Point(55, 2)));
	enemy3->setAnchorPoint(Point::ZERO);
	enemy3->setScale(ENEMY_SCALE_FACTOR);
	enemy3->setFlippedX(true);
	enemy3->retain();

	enemyList.push_back(enemy3);
	this->addChild(enemy3);

	Sprite *enemy4 = Sprite::create("mum.png");
	enemy4->setPosition(level->tileCoordinateToPosition(Point(100, 2)));
	enemy4->setAnchorPoint(Point::ZERO);
	enemy4->setScale(ENEMY_SCALE_FACTOR);
	enemy4->setFlippedX(true);
	enemy4->retain();

	enemyList.push_back(enemy4);
	this->addChild(enemy4);

	Sprite *enemy5 = Sprite::create("mum.png");
	enemy5->setPosition(level->tileCoordinateToPosition(Point(100, 6)));
	enemy5->setAnchorPoint(Point::ZERO);
	enemy5->setScale(ENEMY_SCALE_FACTOR);
	enemy5->setFlippedX(true);
	enemy5->retain();

	enemyList.push_back(enemy5);
	this->addChild(enemy5);

}


void GameScene::updateScene(float delta) {

	cameraTarget->setPositionX(player_sprite->getPosition().x);

	this->updatePlayer(delta);

}

void GameScene::updatePlayer(float delta) {

	if (std::find(heldKeys.begin(), heldKeys.end(), SPACEBAR) != heldKeys.end()) {

		if (player->grounded && player->velocity.y <= 0) {

			player->velocity.y = PLAYER_JUMP_VELOCITY;
			player->state = Player::State::Jumping;
			player->grounded = false;
		}

	}

	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()) {

		player->velocity.x = PLAYER_MAX_VELOCITY;

		if (player->grounded) {
			player->state = Player::State::Walking;
		}

		player->facingRight = true;
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()) {

		player->velocity.x = -PLAYER_MAX_VELOCITY;
		if (player->grounded) {
			player->state = Player::State::Walking;
		}
		player->facingRight = false;
	}

	player->velocity -= Point(0, GRAVITY);
	stutteringFix = 1;

	Rect player_rect = player_sprite->getBoundingBox();

	Point tmp;
	vector<Rect> tiles;
	tiles.clear();

	// center of player's sprite
	tmp = level->positionToTileCoordinate(Point(player->position.x + player->player_size.width * 0.5f,
		player->position.y + player->player_size.height * 0.5f));

	if (player->velocity.x > 0) {
		tiles = level->getCollisionTilesX(tmp, 1);
	}
	else {
		tiles = level->getCollisionTilesX(tmp, -1);
	}

	player_rect.setRect(
		player_sprite->getBoundingBox().getMinX() + player->velocity.x,
		player_sprite->getBoundingBox().getMinY() + 2.0f, // dont let the rectangle touch the ground otherwise, will count as collision
		player->player_size.width,
		player->player_size.height
	);

	for (Rect tile : tiles) {
		if (player_rect.intersectsRect(tile)) {
			player->velocity.x = 0;
			break;
		}
	}

	tiles.clear();

	if (player->velocity.y > 0) {
		tiles = level->getCollisionTilesY(tmp, 1);
	}
	else {
		tiles = level->getCollisionTilesY(tmp, -1);
	}

	player_rect.setRect(
		player_sprite->getBoundingBox().getMinX(),
		player_sprite->getBoundingBox().getMinY(),
		player->player_size.width,
		player->player_size.height
	);

	for (Rect tile : tiles) {

		if (tile.intersectsRect(player_rect)) {
			if (player->velocity.y > 0) {

				player->position.y = tile.getMinY() - player->player_size.height;

			}
			else {

				player->position.y = tile.getMaxY();
				// if we hit the ground, mark us as grounded so we can jump
				player->grounded = true;
				stutteringFix = 0;

			}
			player->velocity.y = 0;
			break;

		}
		player->grounded = false;
	}

	// check for enemy collisions
	for (Sprite *tile : enemyList) {

		if (tile->getBoundingBox().intersectsRect(player_rect)) {

			Point p = level->tileCoordinateToPosition(Point(10, 2));
			player->position.x = p.x;
			player->position.y = p.y;
		}
	}

	player->position.x = player->position.x + player->velocity.x;
	player->position.y = player->position.y + player->velocity.y;

	this->updatePlayerSprite(delta);

	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) == heldKeys.end() && player->grounded) {
		player->velocity.x = 0;
		player->state = Player::State::Standing;
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) == heldKeys.end() && player->grounded) {
		player->velocity.x = 0;
		player->state = Player::State::Standing;
	}

}

void GameScene::updatePlayerSprite(float delta) {

	if (player->state == Player::State::Walking) {

		if (walkRight->isDone()) {
			walkRight->startWithTarget(player_sprite);
		}

		walkRight->step(delta);

		if (player->facingRight) {
			player_sprite->setFlippedX(true);
		}
		else {
			player_sprite->setFlippedX(false);
		}

	}
	else if (player->state == Player::State::Jumping) {

		player_sprite->setSpriteFrame(Sprite::createWithSpriteFrameName("jump")->getSpriteFrame());
		if (player->facingRight) {
			player_sprite->setFlippedX(true);
		}
		else {
			player_sprite->setFlippedX(false);
		}

	}
	else {
		player_sprite->setSpriteFrame(Sprite::createWithSpriteFrameName("idle")->getSpriteFrame());
	}

	player_sprite->setPositionX(player->position.x);
	if (stutteringFix != 0)
		player_sprite->setPositionY(player->position.y);


	/*rectWithBorder->setPositionX(player_sprite->getBoundingBox().getMinX());
	if(stutteringFix != 0){
	rectWithBorder->setPositionY(player->position.y);
	}*/

}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()) {
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
	collidesX = false;
}
GameScene::~GameScene(void)
{
}
