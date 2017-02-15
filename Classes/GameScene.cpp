#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "Global.h"
#include "MainMenuScene.h"

using namespace CocosDenshion;
#define Music "Music.mp3"

Scene* GameScene::createScene()
{
	//creating a scene with physics 
	auto scene = Scene::createWithPhysics();
	//setting up the physics world and taking it from the premade cocos physics
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = GameScene::create();

	// adding the scene to the layer
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// playing hte music whene the level enters from main maneu 
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Music);
	// creating a new level
	level = new Level();
	//loading in the initialized map
	level->loadMap("level1.tmx");
	// retaining the map untill the end of the game
	level->retain();//***relsease later in the code *****

	// setting up the scene director and the size ratios 
	auto visibleSize = Director::getInstance()->getVisibleSize();//init size
	auto director = Director::getInstance();
	level->getMap()->setScale(SCALE_FACTOR);
	// adding the map to the level
	this->addChild(level->getMap());

	//creating a player and retaing it untill i delete it in the deconstuctor
	player = Player::create();
	player->retain();//***relsease later in the code *****

	// setting up a spawn point
	Point point = Point(10, 2);
	player->setPosition(level->tileCoordinateToPosition(point));

	Point origin = Director::getInstance()->getVisibleOrigin();
	Size wsize = Director::getInstance()->getVisibleSize();  //default screen size (or design resolution size, if you are using design resolution)
	Point *center = new Point(wsize.width / 2 + origin.x, wsize.height / 2 + origin.y);
	// setting up a camera 
	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(player->getPositionX()); // set to players x
	cameraTarget->setPositionY(wsize.height / 2 + origin.y); // center of height
	// retaining it till the end
	cameraTarget->retain(); //***relsease later in the code *****

	// adding the player to the layer and updating the game scene
	this->addChild(player);
	this->schedule(schedule_selector(GameScene::updateScene));

	// adding the camera to the layer
	this->addChild(cameraTarget);

	// creating the main menu buttons 
	auto Menu = MenuItemImage::create("Menu.png", "MenuPressed.png", CC_CALLBACK_1(GameScene::GoToMainMenu, this));
	Menu->setPosition(Point(visibleSize.width / 11 + origin.x, visibleSize.height / 1.5 + origin.y));

	//// adding the button to the layer
	auto MenuButton = Menu::create(Menu, NULL);
	MenuButton->setPosition(Point::ZERO);
	this->addChild(MenuButton);

	//making the sprite
	auto house = Sprite::create("home.png"); 
	//setting up the postition
	house->setPosition(Vec2(visibleSize.width* 6.7, visibleSize.height * 0.33));
	
	// getting hte size of the house
	auto houseSize = house->getContentSize();
	// giving the house some physics
	auto physicsBody = PhysicsBody::createBox(Size(houseSize.width, houseSize.height),
		PhysicsMaterial(0.1f, 1.0f, 0.0f));

	// set it to active
	physicsBody->setDynamic(true);
	// add the house to the physics body
	house->setPhysicsBody(physicsBody);
	// add the house to the scene
	this->addChild(house);

	
// setting up the bounding box of the player
	rectWithBorder = DrawNode::create();
	Vec2 vertices[] =
	{
		Vec2(0, player->player_size.height),
		Vec2(player->player_size.width, player->player_size.height),
		Vec2(player->player_size.width, 0),
		Vec2(0,0)
	};

	// giving the bounding box a colour 
	rectWithBorder->drawPolygon(vertices, 4, Color4F(0.0f, 0.3f, 0.3f, 1), 0, Color4F(0.0f, 0.2f, 0.0f, 1));
	// adding the bounding box to the layer
	this->addChild(rectWithBorder);

	// making the camera follow the player using 
	camera = Follow::create(cameraTarget, Rect::ZERO);
	//stay using the camera
	camera->retain();// ****release later in the code****
	//calling the method 
	loadEnemies();
	// run the camera follow
	this->runAction(camera);
	return true;
}

// creating all the enemies in the game
void GameScene::loadEnemies() 
{
	
	Sprite *enemy1 = Sprite::create("mom.png");
	enemy1->setPosition(level->tileCoordinateToPosition(Point(30, 1.8)));
	enemy1->setAnchorPoint(Point::ZERO);
	enemy1->setScale(ENEMY_SCALE_FACTOR);
	enemy1->setFlippedX(true);
	enemy1->retain();

	enemyList.push_back(enemy1);
	this->addChild(enemy1);

	Sprite *enemy2 = Sprite::create("mom.png");
	enemy2->setPosition(level->tileCoordinateToPosition(Point(40, 1.8)));
	enemy2->setAnchorPoint(Point::ZERO);
	enemy2->setScale(ENEMY_SCALE_FACTOR);
	enemy2->setFlippedX(true);
	enemy2->retain();

	enemyList.push_back(enemy2);
	this->addChild(enemy2);


	Sprite *enemy3 = Sprite::create("mom.png");
	enemy3->setPosition(level->tileCoordinateToPosition(Point(45, 1.8)));
	enemy3->setAnchorPoint(Point::ZERO);
	enemy3->setScale(ENEMY_SCALE_FACTOR);
	enemy3->setFlippedX(true);
	enemy3->retain();

	enemyList.push_back(enemy3);
	this->addChild(enemy3);

	Sprite *enemy4 = Sprite::create("mom.png");
	enemy4->setPosition(level->tileCoordinateToPosition(Point(60, 1.8)));
	enemy4->setAnchorPoint(Point::ZERO);
	enemy4->setScale(ENEMY_SCALE_FACTOR);
	enemy4->setFlippedX(true);
	enemy4->retain();

	enemyList.push_back(enemy4);
	this->addChild(enemy4);

	Sprite *enemy5 = Sprite::create("mom.png");
	enemy5->setPosition(level->tileCoordinateToPosition(Point(75, 1.8)));
	enemy5->setAnchorPoint(Point::ZERO);
	enemy5->setScale(ENEMY_SCALE_FACTOR);
	enemy5->setFlippedX(true);
	enemy5->retain();

	enemyList.push_back(enemy5);
	this->addChild(enemy5);

}

// updating the scene
void GameScene::updateScene(float delta) 
{

	cameraTarget->setPositionX(player->getPositionX());

	this->updatePlayer(delta);

}
// updating the player
void GameScene::updatePlayer(float delta) 
{
	// for all the variables with caps please refer to "Global.h"
	// using a vector and using the global varibales that I have set in Global.h
	// if the right arrow is pressed then execuite
	if (std::find(heldKeys.begin(), heldKeys.end(), UP_ARROW) != heldKeys.end())
	{
		// if the grounded and velocit of y vars are less than 0 then..
		if (player->grounded && player->velocity_y <= 0) 
		{
			// make the player jump
			player->velocity_y = PLAYER_JUMP_VELOCITY;
			// player jump bool is true as he is in the air
			player->jumping = true;
			// gorund bool is flase as he is in the air
			player->grounded = false;
		}

	}

	// if the right arrow is pressed then execute
	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end())
	{
		// assign the player x velocity to the max velocity
		player->velocity_x = PLAYER_MAX_VELOCITY;
		
		// set the player facing right bool to be true as he is moving right
		player->facing_right = true;
	}
	// if the left arrow is pressed then execute
	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end())
	{
		// assign the player x velocity to the max velocity
		player->velocity_x = -PLAYER_MAX_VELOCITY;
		// set to flase as we are moving left
		player->facing_right = false;
	}
	// when the player jumps put gravity on him 
	player->velocity_y -= GRAVITY;
	// get the bounding box of the player 
	Rect player_rect = player->getBoundingBox();
	// create a temporay point
	Point tmp;
	vector<Rect> tiles;
	tiles.clear();

	// center of player's sprite
	tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->player_size.width * 0.5f, player->getPositionY() + player->player_size.height * 0.5f));

	// if the player is moving (has veloicty) then he will collide with the tile map
	if (player->velocity_x > 0)
	{
		tiles = level->getCollisionTilesX(tmp, 1);
	}
	else
	{
		tiles = level->getCollisionTilesX(tmp, -1);
	}
	// setting the bounding box
	player_rect.setRect(
		player->getBoundingBox().getMinX() + player->velocity_x,
		player->getBoundingBox().getMinY() + 1.0f, // dont let the rectangle touch the ground otherwise, will count as collision
		player->player_size.width,
		player->player_size.height
	);

	// if the player hits a tile remove the velocity
	for (Rect tile : tiles)
	{
		if (player_rect.intersectsRect(tile)) 
		{
			player->velocity_x = 0;
			break;
		}
	}

	// clear the tiles 
	tiles.clear();

	// if i hit a tile in the air
	if (player->velocity_y > 0) 
	{
		tiles = level->getCollisionTilesY(tmp, 1);
	}
	else if (player->velocity_y < 0)
	{
		tiles = level->getCollisionTilesY(tmp, -1);
	}

	player_rect.setRect(
		player->getBoundingBox().getMinX(),
		player->getBoundingBox().getMinY(),
		player->player_size.width,
		player->player_size.height
	);

	for (Rect tile : tiles)
	{

		if (tile.intersectsRect(player_rect)) 
		{
			if (player->velocity_y > 0) 
			{

				player->setPositionY(player->getPositionY() - player->velocity_y);

			}
			else 
			{

				player->setPositionY(tile.getMaxY());
				// if we hit the ground, mark us as grounded so we can jump
				player->grounded = true;
				player->jumping = false;

			}
			player->velocity_y = 0;
			break;

		}
		player->grounded = false;
	}

	// check for enemy collisions
	for (Sprite *tile : enemyList)
	{

		if (tile->getBoundingBox().intersectsRect(player_rect))
		{

			Point p = level->tileCoordinateToPosition(Point(10, 2));
			player->setPosition(p);
		}
	}

	player->updateState(delta);
	player->velocity_x = 0;

}



// creating the keybord vectors 
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end())
		heldKeys.push_back(keyCode);
	

}

// what happens when the keys are realised
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

// button to go back to the main menu
void GameScene::GoToMainMenu(Ref* pSender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionZoomFlipY::create( 2.0, scene));
}

// when we close the game
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
	// releasing what i retained in the init method
	level->release();
	player->release();
	camera->release();
	cameraTarget->release();

}
