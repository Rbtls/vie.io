#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "SimpleAudioEngine.h"
#include "GPGSManager.h"
USING_NS_CC;

Scene* Scene0::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Scene0::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Scene0::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

	/*auto menu_item_1 = MenuItemAtlasFont::create("Play", "fonts/tuffy_bold_italic-charmap.png",22, 32, 288,
	                                              CC_CALLBACK_1(Scene0::Play,this)); */
	/*auto menu_item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(Scene0::Play, this));*/

	auto menu_item_1 = MenuItemLabel::create(LabelTTF::create("Play", "fonts/jura.ttf", 38), CC_CALLBACK_1(Scene0::Play, this));
	auto menu_item_2 = MenuItemLabel::create(LabelTTF::create("Create Game", "fonts/jura.ttf", 38), CC_CALLBACK_1(Scene0::CreateGame, this));
	auto menu_item_3 = MenuItemLabel::create(LabelTTF::create("Settings", "fonts/jura.ttf", 38), CC_CALLBACK_1(Scene0::Settings, this));
	auto menu_item_4 = MenuItemLabel::create(LabelTTF::create("HowTo", "fonts/jura.ttf", 38), CC_CALLBACK_1(Scene0::HowTo, this));
	auto menu_item_5 = MenuItemLabel::create(LabelTTF::create("Quit", "fonts/jura.ttf", 38), CC_CALLBACK_1(Scene0::Quit, this));
	/*auto menu_item_2 = MenuItemFont::create("Create Game", CC_CALLBACK_1(Scene0::CreateGame, this));
	auto menu_item_3 = MenuItemFont::create("Settings", CC_CALLBACK_1(Scene0::Settings, this));
	auto menu_item_4 = MenuItemFont::create("HowTo", CC_CALLBACK_1(Scene0::HowTo, this));
	auto menu_item_5 = MenuItemFont::create("Quit", CC_CALLBACK_1(Scene0::Quit, this));*/

	menu_item_1->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6) * 5.4));
	menu_item_1->setColor(Color3B(0, 164, 255));


	menu_item_2->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6) * 4.4));
	menu_item_2->setColor(Color3B(0, 164, 255));


	menu_item_3->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6) * 3.4));
	menu_item_3->setColor(Color3B(0, 164, 255));


	menu_item_4->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6) * 2.4));
	menu_item_4->setColor(Color3B(0, 164, 255));


	menu_item_5->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 6) * 1.4));
	menu_item_5->setColor(Color3B(0, 164, 255));

	auto *menu = Menu::create(menu_item_1, menu_item_2, menu_item_3, menu_item_4, menu_item_5, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	do {
      CC_BREAK_IF(!CCLayerColor::initWithColor(Color4B(255,255,255,255)));
	} while (0);
	return true;
}

void Scene0::Play(cocos2d::Ref *pSender)
{
	CCLOG("Play");
	if (GPGSManager::IsSignedIn()) {
         CCLOG("! Signed In !");
		 g_engine.ShowRoomInbox();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
	     g_engine.ShowRoomInbox();
    }
	auto scene = Scene1::createScene();
	Director::getInstance()->pushScene(scene);
}
void Scene0::CreateGame(cocos2d::Ref *pSender)
{
	CCLOG("Create Game");
	if (GPGSManager::IsSignedIn()) {
         CCLOG("! Signed In !");
		 g_engine.InviteFriend();
		 //g_engine.QuickMatch();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
	     g_engine.InviteFriend();
		//g_engine.QuickMatch();
    }
	auto scene = Scene1::createScene();
	Director::getInstance()->pushScene(scene);
}
void Scene0::Settings(cocos2d::Ref *pSender)
{
	/////////////////////////////////////////////////////////////             TEST
	if (GPGSManager::IsSignedIn()) {
         CCLOG("! Signed In !");
		 g_engine.QuickMatch();
    }
    else{
        GPGSManager::BeginUserInitiatedSignIn();
	     g_engine.QuickMatch();
    }
	auto scene = Scene1::createScene();
	Director::getInstance()->pushScene(scene);
///////////////////////////////////////////////////////////////

	CCLOG("Settings");
}
void Scene0::HowTo(cocos2d::Ref *pSender)
{
	CCLOG("HowTo");
}
void Scene0::Quit(cocos2d::Ref *pSender)
{
	CCLOG("Quit");
	Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	    exit(0);
	#endif
}


void Scene0::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
