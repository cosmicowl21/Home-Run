#include "AppDelegate.h"

#include "GameScene.h"

USING_NS_CC;
static cocos2d::Size designResolutionSize = cocos2d::Size(1350, 768);// change the resoulation size of the ouput screen 
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() 
{

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) 
	{
	// if the target platform is any of the below then use the screen size set for window size
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		glview = GLViewImpl::createWithRect("Home Run", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("Home_Run");
#endif
		director->setOpenGLView(glview);
	}

	director->setDisplayStats(true);

	director->setAnimationInterval(1.0 / 60);

	auto scene = GameScene::createScene();

	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}