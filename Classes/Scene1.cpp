#include "Scene1.h"
#include "Scene0.h"
#include "SimpleAudioEngine.h"
#include "AdmobHelper.h"
#include <jni.h>
#include <errno.h>
#include <android/sensor.h>

#include "gpg/android_platform_configuration.h"
#include "gpg/android_initialization.h"
#include "gpg/android_support.h"
#include "gpg/debug.h"

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <cpu-features.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include "Anim.h"
//#include "StateManager.h"
#include "GPGSManager.h"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

USING_NS_CC;

Scene* Scene1::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Scene1::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Scene1::init()
{
	/*projectile = cocos2d::Sprite::create("projectile.png");
  addChild(projectile);
  projectile2 = cocos2d::Sprite::create("projectile.png");
  addChild(projectile2);*/
  //////////////////////////////
  // 1. super init first
  if ( !Layer::init() )
  {
      return false;
  }
	
	labelTouchInfo = Label::createWithCharMap("fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
  labelTouchInfo->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,
                  Director::getInstance()->getVisibleSize().height ));
	addChild(labelTouchInfo);

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("HelloWorld.png", "CloseSelected.png",
		CC_CALLBACK_1(Scene1::menuCloseCallback, this));
  closeItem->setScale(0.5f);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
	auto winSize = Director::getInstance()->getVisibleSize();
	
  /*auto _player = Sprite::create("mysprite.png");
	_player->setPosition(Vec2(winSize.width * 0.15, winSize.height * 0.5));
	this->addChild(_player, 2);*/
	
  const auto factory = dragonBones::CCFactory::getFactory();
  factory->loadDragonBonesData("mecha_1502b/mecha_1502b_ske.json");
  factory->loadTextureAtlasData("mecha_1502b/mecha_1502b_tex.json");
  factory->loadDragonBonesData("skin_1502b/skin_1502b_ske.json");
  factory->loadTextureAtlasData("skin_1502b/skin_1502b_tex.json");
  factory->loadDragonBonesData("weapon_1000/weapon_1000_ske.json");
  factory->loadTextureAtlasData("weapon_1000/weapon_1000_tex.json");
  auto _player = new Anim(winSize);
  this->addChild(_player->_getArmatureDisplay());


  //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB5A1);
	
	/*auto closeItem = MenuItemImage::create(
		"HelloWorld.png",
		"HelloWorld.png",
		CC_CALLBACK_1(Scene1::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	this->addChild(closeItem);*/
	
	//show banner
    auto _joystick = Sprite::create("joystick.png");
    _joystick->setPosition(Vec2(winSize.width*0.14, winSize.height*0.33));
    _joystick->setScale(2);
    _joystick->setOpacity(70);
    this->addChild(_joystick, 3);
    
    //create listener for joystick
    auto listener_joystick = EventListenerTouchOneByOne::create();
    listener_joystick->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_joystick, _joystick);
   
    //AdmobHelper::showAd();

    /*auto listener1 = EventListenerCustom::create("dispatchReceivedCoordinates", CC_CALLBACK_1(Scene1::messageReceived, this));
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, _player);
    
	  auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);*/

	return true;
}


bool Scene1::onTouchBegan(Touch* touch, Event* unused_event) {
  


  //How to send udp message with projectile info
  /*projectile = cocos2d::Sprite::create("projectile.png");
  addChild(projectile);
  std::stringstream ss;
  // 1  - Just an example for how to get the  _player object
  auto _player = unused_event->getCurrentTarget();
 
  // 2
  Vec2 touchLocation = touch->getLocation();
  Vec2 offset = touchLocation - _player->getPosition();
 
  // 3
  if (offset.x < 0) {
    return true;
  }
 
  // 4
  projectile->setPosition(_player->getPosition());
 
  // 5
  offset.normalize();
  auto shootAmount = offset * 1000;
 
  // 6
  auto realDest = shootAmount + projectile->getPosition();
 
  // 7
  auto actionMove = MoveTo::create(2.0f, realDest);
  auto actionRemove = RemoveSelf::create();
  projectile->runAction(Sequence::create(actionMove,actionRemove, nullptr));
  
  
  std::vector<uint8_t> v;  
  v.push_back('C');
  //float f = realDest.x;
  //char *a = (char *) &f;
  unsigned char a[sizeof(float)];
  memcpy(a, &realDest.x, sizeof(float));
  v.push_back(a);
  memcpy(a, &realDest.y, sizeof(float));
  f = realDest.y;
  v.push_back(a); * /
  
  Scene1::convertToUint8(realDest.x, v);
  v.push_back('#');
  Scene1::convertToUint8(realDest.y, v);
  
  ss << realDest.x << " " << realDest.y;
  labelTouchInfo->setString(ss.str().c_str());
  g_engine.gameServices->RealTimeMultiplayer().SendUnreliableMessageToOthers(g_engine.room_, v);*/
  
  return true;
}

void Scene1::messageReceived(Event* unused_event)
{	
  /*projectile2 = cocos2d::Sprite::create("projectile.png");
  addChild(projectile2);
  auto _player = unused_event->getCurrentTarget();
  projectile2->setPosition(_player->getPosition());
  auto actionMove1 = MoveTo::create(2.0f, g_engine.gotDest);
  auto actionRemove1 = RemoveSelf::create();
  projectile2->runAction(Sequence::create(actionMove1,actionRemove1, nullptr));*/
}

void Scene1::menuCloseCallback(Ref* pSender)
{
	//show fullscreen interstitial
//	AdmobHelper::showfullscreenAd();
    AdmobHelper::hideAd();
    auto scene = Scene0::createScene();
    Director::getInstance()->replaceScene(scene);
    g_engine.LeaveGame();
  
  /*  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif*/
}
 
void Scene1::convertToUint8 (float f, std::vector<unsigned char>& v)
{
  float left = (int)f;
  float right = f - left; //getting decimal part
  if (f<0)
  {
     v.push_back('-'); 
     left*=(-1);
     right*=(-1);
  }
  if (f==0)
  {
     v.push_back('0'); 
  }
  while (left!=0)
  {
    while(left>10)
    {
       left/=10;
    }
    v.push_back(((int)left)+'0');
    left=left-(int)left; 
    left*=10;
    left=round(left*100)/100; //rounding 
  }
  v.push_back('.'); //adding comma
  while (right!=0)
  {
     right=right*10;
     right=round(right*100)/100;
     v.push_back(((int)right)+'0');
     right=right-(int)right; 
  }   
}

