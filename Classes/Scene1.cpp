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
#include "renderer/CCTexture2D.h"
//#include "Anim.h"
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
	
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("HelloWorld.png", "CloseSelected.png",
		CC_CALLBACK_1(Scene1::menuCloseCallback, this));
  closeItem->setScale(0.5f);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + visibleSize.height - closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
		
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
  auto _player = new Anim(visibleSize);
  this->addChild(_player->_getArmatureDisplay());

  //adding 3d map
  cube3D = Sprite3D::create("2.obj");
  cube3D->setScale(14);
  cube3D->setRotation3D(Vec3(0,90,0));
  cube3D->setGlobalZOrder(-10);

  //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB5A1);
	
	/*auto closeItem = MenuItemImage::create(
		"HelloWorld.png",
		"HelloWorld.png",
		CC_CALLBACK_1(Scene1::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
	origin.y + closeItem->getContentSize().height / 2));
	this->addChild(closeItem);*/
	
	//creating joystick
  auto _joystick = Sprite::create("joystick.png");
  _joystick->setPosition(Vec2(visibleSize.width*0.14, visibleSize.height*0.33));
  _joystick->setScale(2);
  _joystick->setOpacity(70);
  
  //creating listener for joystick
  auto listener_joystick = EventListenerTouchOneByOne::create();
  listener_joystick->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this, _player, _joystick, cube3D);
  listener_joystick->onTouchMoved = CC_CALLBACK_2(Scene1::onTouchMoved, this, _player, _joystick, cube3D);
  listener_joystick->onTouchEnded = CC_CALLBACK_2(Scene1::onTouchEnded, this, _player, _joystick, cube3D);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_joystick, _joystick);
  
  this->addChild(_joystick, 3);
  
  //creating aim
  auto _aim = Sprite::create("joystick.png");
  _aim->setPosition(Vec2(visibleSize.width*0.85, visibleSize.height*0.33));
  _aim->setScale(2);
  _aim->setOpacity(70);

  //creating listener for aim
  auto listener_aim = EventListenerTouchOneByOne::create();
  listener_aim->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this, _player, _aim, cube3D);
  listener_aim->onTouchMoved = CC_CALLBACK_2(Scene1::onTouchMoved, this, _player, _aim, cube3D);
  listener_aim->onTouchEnded = CC_CALLBACK_2(Scene1::onTouchEnded, this, _player, _aim, cube3D);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_aim, _aim);
  
  this->addChild(_aim, 3);

  //cube3D->setTexture("brickwork-texture.jpg");
/*  cocos2d::Image *im = new Image();
  im->initWithImageFile("brickwork-texture.jpg");
    
  Texture2D *t2 = new Texture2D();
  t2->initWithImage(im);
  cocos2d::Texture2D::TexParams texParam;
  texParam.wrapS = GL_REPEAT;
  texParam.wrapT = GL_REPEAT;
  texParam.minFilter = GL_LINEAR;
  texParam.magFilter = GL_LINEAR;

  t2->setTexParameters(texParam);
  t2->setMaxS(10.0f);
  t2->setMaxT(10.0f);
  t2->retain();
  cube3D->setTexture(t2);*/
  //cube3D->setRotation3D(Vec3(180,45,0));
  cube3D->setPosition(Vec2(visibleSize.width/2 /*+ origin.x*/, visibleSize.height/4 /*+ origin.y*/));
  this->addChild(cube3D, 1);

  //adding light to scene
  //auto light = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B::RED);
  //addChild(light);

  //creating skybox
  auto box = Skybox::create();
  //creating skybox texture
  auto textureCube = TextureCube::create("skybox/left.jpg",  "skybox/right.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg");
  
  // set cube map texture parameters
  Texture2D::TexParams tRepeatParams;
  tRepeatParams.magFilter = GL_NEAREST;
  tRepeatParams.minFilter = GL_NEAREST;
  tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
  tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
  textureCube->setTexParameters(tRepeatParams);

  // setting textureCube for Skybox
  //box->setTexture(textureCube);
  // create and set our custom shader
  //auto shader = GLProgram::createWithFilenames("cubemap.vert", "cubemap.frag");
  //auto state = GLProgramState::create(shader);

  // bind cube map texture to uniform
  //state->setUniformTexture("u_cubeTex", textureCube);
  box->setTexture(textureCube);
  this->addChild(box);

  //cube3D->setForce2DQueue(true); // required, this put the model in the same render queue as the RenderTextures
 
  //Scene1::basicShader(cube3D);

 /* // RenderTextures
  renderTexDefault = RenderTexture::create(1280, 720, Texture2D::PixelFormat::RGBA8888);
  this->addChild(renderTexDefault, 1);
  renderTexDefault->setPosition(1280/2, 720/2);
  renderTexDefault->setKeepMatrix(true); // required

  renderTexWithBuffer = RenderTexture::create(1280, 720, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
  this->addChild(renderTexWithBuffer, 1);
  renderTexWithBuffer->setPosition(1280/2, 720/2);
  renderTexWithBuffer->setKeepMatrix(true); // required*/

  //adding camera
  /*
  Size s = Director::getInstance()->getVisibleSize();
  auto camera =Camera::createPerspective(60, s.width/s.height, 0.1f, 200);
  camera->setCameraFlag(CameraFlag::USER1);
  */


  //show banner
  //AdmobHelper::showAd();

  /*auto listener1 = EventListenerCustom::create("dispatchReceivedCoordinates", CC_CALLBACK_1(Scene1::messageReceived, this));
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, _player);
  
	auto eventListener = EventListenerTouchOneByOne::create();
  eventListener->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener /*->clone() * / , _player);*/

	return true;
}

bool Scene1::onTouchBegan(Touch* touch, Event* unused_event, Anim* _player, Sprite* _node, Sprite3D* _map) {

  auto bounds = unused_event->getCurrentTarget()->getBoundingBox();
  
  auto location = touch->getLocation();

  auto moveR = RepeatForever::create(MoveBy::create(0, Vec2(-1,0)));
  auto moveL = RepeatForever::create(MoveBy::create(0, Vec2(1,0)));
  auto moveF = RepeatForever::create(MoveBy::create(0, Vec3(0,0,1)));
  auto moveB = RepeatForever::create(MoveBy::create(0, Vec3(0,0,-1)));
 
  std::stringstream ss;

  if (bounds.containsPoint(location)) {
    //checking whether the circle is located to the left of the screen (that means it's a joystick)
    if ((unused_event->getCurrentTarget()->getPositionX()) < (Director::getInstance()->getVisibleSize().width / 2)) {
      _player->move(1);
      // get the location of the touch relative to your button
      auto nodeSpaceLocation = _node->getParent()->convertToNodeSpace(location);
      
      //debug info about touch coordinates inside the sprite
      //ss << nodeSpaceLocation.x << " " << nodeSpaceLocation.y;
      //labelTouchInfo->setString(ss.str().c_str());

      if(nodeSpaceLocation.x < 50 && nodeSpaceLocation.y < 50) {
        _map->runAction(moveL);
        _map->runAction(moveB);
        move_state = 14;
      }
      else if (nodeSpaceLocation.x < 50 && nodeSpaceLocation.y > 50) {
        _map->runAction(moveL);
        _map->runAction(moveF);
        move_state = 13;
      }
      else if (nodeSpaceLocation.x > 50 && nodeSpaceLocation.y < 50) {
        _map->runAction(moveR);
        _map->runAction(moveB);
        move_state = 24;
      }
      else if (nodeSpaceLocation.x > 50 && nodeSpaceLocation.y > 50) {
        _map->runAction(moveR);
        _map->runAction(moveF);
        move_state = 23;
      }
      else if (nodeSpaceLocation.x < 50 && nodeSpaceLocation.y == 50) {
        _map->runAction(moveL);
        move_state = 1;
      }
      else if (nodeSpaceLocation.x > 50 && nodeSpaceLocation.y == 50) {
        _map->runAction(moveR);
        move_state = 2;
      }
      else if (nodeSpaceLocation.y < 50 && nodeSpaceLocation.x == 50) {
        _map->runAction(moveB);
        move_state = 4;
      }
      else if (nodeSpaceLocation.y > 50 && nodeSpaceLocation.x == 50) {
        _map->runAction(moveF);
        move_state = 3;
      }
    }
    //checking whether the circle is located on the right side of the screen (that means it's an aim circle)
    else if ((unused_event->getCurrentTarget()->getPositionX()) > (Director::getInstance()->getVisibleSize().width / 2)) {
      _player->attack(true);
    }
  }
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

void Scene1::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Sprite3D* _map)
{
  auto bounds = event->getCurrentTarget()->getBoundingBox();
  // checking only if the joystick has been released because the stop animation is related to movement
  if (bounds.containsPoint(touch->getLocation())) {
    if ((event->getCurrentTarget()->getPositionX()) < (Director::getInstance()->getVisibleSize().width / 2)) {
      _player->stop();
      
      //stops repeated movement from onTouchBegin
      _map->stopAllActions();
    }
    else if ((event->getCurrentTarget()->getPositionX()) > (Director::getInstance()->getVisibleSize().width / 2)) {
      _player->attack(false);

    }
  }
}

void Scene1::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Sprite3D* _map)
{
  auto bounds = event->getCurrentTarget()->getBoundingBox();
  
  auto location = touch->getLocation();

  auto moveR = RepeatForever::create(MoveBy::create(0, Vec2(-1,0)));
  auto moveL = RepeatForever::create(MoveBy::create(0, Vec2(1,0)));
  auto moveF = RepeatForever::create(MoveBy::create(0, Vec3(0,0,1)));
  auto moveB = RepeatForever::create(MoveBy::create(0, Vec3(0,0,-1)));
    
  std::stringstream ss;

  if (bounds.containsPoint(touch->getLocation())) {
    if ((event->getCurrentTarget()->getPositionX()) < (Director::getInstance()->getVisibleSize().width / 2)) {
            
      //stops repeated movement from onTouchBegin
      //_map->stopAllActions();
      //changing movement
      switch (move_state) {
        case 14:
          _map->runAction(moveR);
          _map->runAction(moveF);
          break;
        case 13:
          _map->runAction(moveR);
          _map->runAction(moveB);
          break;
        case 24:
          _map->runAction(moveL);
          _map->runAction(moveF);
          break;
        case 23:
          _map->runAction(moveL);
          _map->runAction(moveB);
          break;
        case 1:
          _map->runAction(moveR);
          break;
        case 2:
          _map->runAction(moveL);
          break;
        case 3:
          _map->runAction(moveB);
          break;
        case 4:
          _map->runAction(moveF);
          break;
      }

      // get the location of the touch relative to your button
      auto nodeSpaceLocation = _node->getParent()->convertToNodeSpace(location);
      
      ss << nodeSpaceLocation.x << " " << nodeSpaceLocation.y;
      labelTouchInfo->setString(ss.str().c_str());

      if (nodeSpaceLocation.x < 50 && nodeSpaceLocation.y < 50) {
       // if (move_state != 14) {
          _map->runAction(moveL);
          _map->runAction(moveB);
          move_state = 14;
       // }
      }
      else if (nodeSpaceLocation.x < 50 && nodeSpaceLocation.y > 50) {
        //if (move_state != 13) {
          _map->runAction(moveL);
          _map->runAction(moveF);
          move_state = 13;
        //}
      }
      else if (nodeSpaceLocation.x > 50 && nodeSpaceLocation.y < 50) {
        //if (move_state != 24) {
          _map->runAction(moveR);
          _map->runAction(moveB);
          move_state = 24;
        //}
      }
      else if (nodeSpaceLocation.x > 50 && nodeSpaceLocation.y > 50) {
        //if (move_state != 23) {
          _map->runAction(moveR);
          _map->runAction(moveF);
          move_state = 23;
        //}
      }
      else if (nodeSpaceLocation.x < 50 && nodeSpaceLocation.y == 50) {
        //if (move_state != 1) {
          _map->runAction(moveL);
          move_state = 1;
        //}
      }
      else if (nodeSpaceLocation.x > 50 && nodeSpaceLocation.y == 50) {
        //if (move_state != 2) {
          _map->runAction(moveR);
          move_state = 2;
        //}
      }
      else if (nodeSpaceLocation.y < 50 && nodeSpaceLocation.x == 50) {
        //if (move_state != 4) {
          _map->runAction(moveB);
          move_state = 4;
        //}
      }
      else if (nodeSpaceLocation.y > 50 && nodeSpaceLocation.x == 50) {
        //if (move_state != 3) {
          _map->runAction(moveF);
          move_state = 3;
        //}
      }
    }
    else if ((event->getCurrentTarget()->getPositionX()) > (Director::getInstance()->getVisibleSize().width / 2)) {
      _player->attack(false);

    }
  }
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

void Scene1::basicShader(cocos2d::Sprite3D * sprite3d)
{
    if(sprite3d)
    {
        GLProgram * p = new GLProgram();
        p->initWithFilenames("shader.vert", "shader.frag");
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        sprite3d->setGLProgram(p);
    }
}
