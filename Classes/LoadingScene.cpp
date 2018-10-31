#include "LoadingScene.h"
#include "Scene0.h"
#include "GPGSManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* LoadingScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoadingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 3. add your codes below...

    //auto loadtext = LabelTTF::create("Loading...", "fonts/Streetwear.ttf", 20);
    auto loadtext = Label::createWithSystemFont("Loading...", "fonts/Streetwear.ttf", 20);
    loadtext->setHeight(100);
    loadtext->setVerticalAlignment(TextVAlignment::CENTER);
    loadtext->setPosition(Point(visibleSize.width / 1.25, (visibleSize.height / 6) * 1.3));
	loadtext->setColor(Color3B(252, 141, 215));
    this->addChild(loadtext);    

    do {
      CC_BREAK_IF(!CCLayerColor::initWithColor(Color4B(255,255,255,255)));
	} while (0);

 	return true;
}


void LoadingScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}