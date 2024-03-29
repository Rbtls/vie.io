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
#include <math.h>
#include "CCRef.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCRenderState.h"
#include "3d/CCBundle3D.h"
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
    auto scene = Scene::createWithPhysics();
    
    scene->getPhysics3DWorld()->setGravity(Vec3(0,-10,0));
    
    // 'layer' is an autorelease object
    auto layer = Scene1::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Scene1::init() {
	/*projectile = cocos2d::Sprite::create("projectile.png");
  addChild(projectile);
  projectile2 = cocos2d::Sprite::create("projectile.png");
  addChild(projectile2);*/
  //////////////////////////////
  // 1. super init first
  if ( !Layer::init() ) {
      return false;
  }
  //Director::getInstance()->setDepthTest(true);
  Director::getInstance()->setDisplayStats(false);

  hudlayer = Layer::create();
  
  world = Layer::create();
  world->setScale(0.2f);

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  
  Camera::getDefaultCamera()->setScaleZ(5.f);

  // creating 1st camera for everything except the hud sprites
  auto _camera = Camera::createPerspective(60, (float)visibleSize.width/visibleSize.height, 1.0, 1000);
  _camera->setPosition3D(Vec3(visibleSize.width/2 + origin.x,
                        visibleSize.height/2 + origin.y,
                        0)+Vec3(0, 0, 250));
  _camera->lookAt(Vec3(visibleSize.width/2 + origin.x,
                        visibleSize.height/2 + origin.y,
                        0), Vec3(0, 1, 0));
  _camera->setCameraFlag(CameraFlag::USER1);
  _camera->setScaleZ(5.f);
  world->setCameraMask((unsigned short)CameraFlag::USER1);
  this->addChild(_camera);
  
  // creating 2nd camera for HUD
  auto _HUD = Camera::createPerspective(60, (float)visibleSize.width/visibleSize.height, 1.0, 1000);
  // set parameters for HUD camera
  _HUD->setPosition3D(Vec3(visibleSize.width/2 + origin.x,
                        visibleSize.height/2 + origin.y,
                        0)+Vec3(0, 0, 250));
  _HUD->lookAt(Vec3(visibleSize.width/2 + origin.x,
                        visibleSize.height/2 + origin.y,
                        0), Vec3(0, 1, 0));
  _HUD->setCameraFlag(CameraFlag::USER2);
  
  hudlayer->setCameraMask((unsigned short)CameraFlag::USER2);
  this->addChild(_HUD); 
	
  //debug info
	/*labelTouchInfo = Label::createWithCharMap("fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
  labelTouchInfo->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,
                  Director::getInstance()->getVisibleSize().height ));*/
	//world->addChild(labelTouchInfo);

  //creating sprite for touch
  star = Sprite::create("touch.png");
  star->setScale(0.15f);
  star->setOpacity(80);
  star->setVisible(false);
  hudlayer->addChild(star);   

  aim_star = Sprite::create("touch.png");
  aim_star->setScale(0.15f);
  aim_star->setOpacity(80);
  aim_star->setVisible(false);
  hudlayer->addChild(aim_star);   

  auto _player = new Anim(visibleSize);

  // add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("HelloWorld.png", "CloseSelected.png",
		CC_CALLBACK_1(Scene1::menuCloseCallback, this, _player, world, hudlayer));
  closeItem->setScale(0.5f);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + visibleSize.height - closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	hudlayer->addChild(menu, 1);

  /////////////////////////////
  // 3. add your codes below...
		
  /*auto _player = Sprite::create("mysprite.png");
	_player->setPosition(Vec2(winSize.width * 0.15, winSize.height * 0.5));
	this->addChild(_player, 2);*/
  /*
  Physics3DRigidBodyDes rbDes;
  auto ball = Sprite3D::create("ball.c3b");
  rbDes.mass = 4.0f;
  rbDes.shape = Physics3DShape::createSphere((float)(ball->getContentSize().width/2)); //radius
  auto ballBody = Physics3DRigidBody::create(&rbDes);
  auto ballComponent = Physics3DComponent::create(ballBody);
  ball->setScale(5);
  ball->addComponent(ballComponent);
  ballComponent->syncPhysicsToNode();
  ball->setTexture("skybox/112.jpg");
  ball->setGlobalZOrder(-10);
  ballComponent->syncNodeToPhysics();
  ball->setPosition(Vec2(visibleSize.width/2, visibleSize.height));
  //ballComponent->syncNodeToPhysics();
  //ball->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
  ball->setCameraMask((unsigned short)CameraFlag::USER1);
  world->addChild(ball, 2);
  */

  _player->_getArmatureDisplay()->setCameraMask((unsigned short)CameraFlag::USER1);
  
  //adding series of sprites from one armature into the billboard object
  Physics3DColliderDes rbDesBill;
  //Physics3DRigidBodyDes rbDesBill;
  auto billboard = BillBoard::create(BillBoard::Mode::VIEW_PLANE_ORIENTED);
  billboard->setCameraMask((unsigned short)CameraFlag::USER1);
  billboard->addChild(_player->_getArmatureDisplay());
  //rbDesBill.mass = 0.0f;
  rbDesBill.shape = Physics3DShape::createSphere(2.0f);
  //LOGI("!!!!!!!!!!!!!!!!!!!!!!!maxX:%f minX:%f maxY:%f minY:%f sizeX:%f sizeY:%f", _player->_getArmatureDisplay()->getBoundingBox().getMaxX(), _player->_getArmatureDisplay()->getBoundingBox().getMinX(), _player->_getArmatureDisplay()->getBoundingBox().getMaxY(), _player->_getArmatureDisplay()->getBoundingBox().getMinY(), _player->_getArmatureDisplay()->getBoundingBox().size.width, _player->_getArmatureDisplay()->getBoundingBox().size.height);
  //rbDesBill.isTrigger = true;
  auto billBody = Physics3DCollider::create(&rbDesBill);
  //auto billBody = Physics3DRigidBody::create(&rbDesBill);
  //billBody->setKinematic(true);
  auto billComponent = Physics3DComponent::create(billBody);
  billboard->addComponent(billComponent);
  billComponent->syncPhysicsToNode();
  billComponent->syncNodeToPhysics();
  billboard->setPosition(cocos2d::Vec2(visibleSize.width/2, visibleSize.height/3));
  world->addChild(billboard);

  //adding 3d map
  Physics3DRigidBodyDes rbDesMap;
  //Physics3DColliderDes rbDesMap;
  std::vector<Vec3> trianglesList = Bundle3D::getTrianglesList("2.obj");
  float scale = (4.f);  //3,66*12=44
  for (auto& it : trianglesList) {
    it *= scale;
  }
  rbDesMap.mass = 0.0f;
  rbDesMap.shape = Physics3DShape::createMesh(&trianglesList[0], (int)trianglesList.size() / 3);
  cube3D = Sprite3D::create("2.obj");
  auto mapBody = Physics3DRigidBody::create(&rbDesMap);
  //mapBody->setGravity(1);
  //auto mapBody = Physics3DCollider::create(&rbDesMap);
  //rbDesMap.isTrigger = true;
  //mapBody->setRestitution(1.0f);
  mapBody->setKinematic(true);
  auto mapComponent = Physics3DComponent::create(mapBody);
  //auto cubeMat = Material::createWithFilename("2.material");
  //cube3D->setMaterial(cubeMat);
  cube3D->setScale(40);
  cube3D->addComponent(mapComponent);
  mapComponent->syncPhysicsToNode();
  cube3D->setRotation3D(Vec3(0,90,0));
  int n = -63; //-61 scale=44
  cube3D->setPosition(Vec2(30, n));
  mapComponent->syncNodeToPhysics();
  //mapComponent->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);
  cube3D->setCameraMask((unsigned short)CameraFlag::USER1);
  
  //creating rotation point
  rotationPoint =  Node::create();
  //adding 3d map as a child to the rotation point
  rotationPoint->addChild(cube3D);
  _player->_getArmatureDisplay()->addChild(rotationPoint);
  rotationPoint->setPosition3D(Vec3(_player->_getArmatureDisplay()->getPosition3D().x, _player->_getArmatureDisplay()->getPosition3D().y, _player->_getArmatureDisplay()->getPosition3D().z));

  // creating collisions
  billBody->setCollisionCallback([=](const Physics3DCollisionInfo &ci) {
    int speed = 6;
    if (ci.objB == mapBody) {
      //LOGI("!!!!!!!!!!!!!!!!!!!!!!!Collision Point Pos: %f, %f, %f", ci.collisionPointList.back().localPositionOnA.x, ci.collisionPointList.back().localPositionOnA.y, ci.collisionPointList.back().localPositionOnA.z );
      //1-L, 2-R, 3-F, 4-B, 13-LF, 14-LB, 23-RF, 24-RB move_state
      //changing map position depending on move_state
      switch (move_state) {
      case 14:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(-speed,0,speed)));
        break;
      case 13:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(-speed,0,-speed)));
        break;
      case 24:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(speed,0,speed)));
        break;
      case 23:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(speed,0,-speed)));
        break;
      case 1:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(-speed,0,0)));
        break;
      case 2:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(speed,0,0)));
        break;
      case 3:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(0,0,-speed)));
        break;
      case 4:
        cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(0,0,speed)));
        break;
      case 0:
        break;
      }
      //cube3D->setPosition3D(ci.collisionPointList.back().worldPositionOnB - Vec3(110,210,0));
    }
  });

  //creating skybox
  box = Sprite3D::create("skybox/skybox.obj");
  box->setScale(520);
  box->setPosition(cocos2d::Vec2(visibleSize.width/2, visibleSize.height/2));
  box->setLightMask(0);
  //creating skybox texture
  //auto box = Skybox::create();
  //auto textureCube = TextureCube::create("skybox/right.jpg",  "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg");
  // set cube map texture parameters
  //Texture2D::TexParams tRepeatParams;
  //tRepeatParams.magFilter = GL_NEAREST;
  //tRepeatParams.minFilter = GL_NEAREST;
  //tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
  //tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
  //box->setTexParameters(tRepeatParams);

  //box->setTexture(textureCube);
  box->setCameraMask((unsigned short)CameraFlag::USER1);
  this->addChild(box, 2);
 
  //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB5A1);
	
	//creating joystick
  _joystick = Sprite::create("joystick.png");
  _joystick->setScale(0.91);
  _joystick->setOpacity(90);
  _joystick->setPosition(Vec2(_joystick->getBoundingBox().size.width/2, _joystick->getBoundingBox().size.height/1.3));
  //getting initial joystick's coordinates in order to modify them after the rotation of the map is completed (so it would be possible to control character against map directions, not against camera)
  coord = _joystick->getPosition3D();
  Up.y = coord.y + (_joystick->getBoundingBox().size.height/2);
  Down.y = coord.y - (_joystick->getBoundingBox().size.height/2);
  Left.x = coord.x - (_joystick->getBoundingBox().size.width/2);
  Right.x = coord.x + (_joystick->getBoundingBox().size.width/2);
  Up.x = coord.x;
  Down.x = coord.x;
  Left.y = coord.y;
  Right.y = coord.y;

  //creating listener for joystick
  auto listener_joystick = EventListenerTouchOneByOne::create();
  listener_joystick->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this, _player, _joystick, _camera, billboard);
  listener_joystick->onTouchMoved = CC_CALLBACK_2(Scene1::onTouchMoved, this, _player, _joystick, _camera, billboard);
  listener_joystick->onTouchEnded = CC_CALLBACK_2(Scene1::onTouchEnded, this, _player, _joystick, _camera, billboard);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_joystick, _joystick);
  
  hudlayer->addChild(_joystick, 3);
  
  //creating aim circle
  _aim = Sprite::create("aim.png");
  _aim->setPosition(Vec2(visibleSize.width*0.8, visibleSize.height*0.23));
  _aim->setScale(2.5);
  _aim->setOpacity(70);
 
  //creating listener for aim
  auto listener_aim = EventListenerTouchOneByOne::create();
  listener_aim->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this, _player, _aim, _camera, billboard);
  listener_aim->onTouchMoved = CC_CALLBACK_2(Scene1::onTouchMoved, this, _player, _aim, _camera, billboard);
  listener_aim->onTouchEnded = CC_CALLBACK_2(Scene1::onTouchEnded, this, _player, _aim, _camera, billboard);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_aim, _aim);
  
  hudlayer->addChild(_aim, 3);
  
  //cube3D->setForce2DQueue(true); // required, this'll put the model in the same render queue as the RenderTextures
 
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

  /*auto listener1 = EventListenerCustom::create("dispatchReceivedCoordinates", CC_CALLBACK_1(Scene1::messageReceived, this));
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, _player);
  
	auto eventListener = EventListenerTouchOneByOne::create();
  eventListener->onTouchBegan = CC_CALLBACK_2(Scene1::onTouchBegan, this);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener /*->clone() * / , _player);*/
  
  //adding ambient light for skybox
  auto lightAmb = AmbientLight::create(Color3B::WHITE);
  lightAmb->setLightFlag(LightFlag::LIGHT1);
  world->addChild(lightAmb);
  
  //adding map light 
  //auto light = SpotLight::create(Vec3(-1.0f, -1.0f, 0.0f), Vec3(25.0f, 23.0f, 0.0f), Color3B::WHITE, 0.0, 10.f, 1000000.0f) ;
  //auto light = DirectionLight::create(Vec3(-1.0f, -1.0f, -0.4f), Color3B::WHITE);
  auto light = PointLight::create(Vec3(0.0f, 24.0f, 0.0f), Color3B::WHITE, 10000000000.f);
  light->setLightFlag(LightFlag::LIGHT0);
  light->setIntensity(1.3f);
  cube3D->setLightMask(1);
  cube3D->addChild(light);

  //show banner
  //AdmobHelper::showAd();

  //initializing map rotation angle
  angle = 0;

  this->addChild(world);
  this->addChild(hudlayer);

	return true;
}

bool Scene1::onTouchBegan(Touch* touch, Event* event, Anim* _player, Sprite* _node, Camera* _camera, BillBoard* billboard) {
  
  auto bounds = event->getCurrentTarget()->getBoundingBox();
  
  location = touch->getLocation();
  
  // get the location of the touch relative to your button
  nodeSpaceLocation = _node->getParent()->convertToNodeSpace(location);

  if (bounds.containsPoint(location)) {
    //checking whether the circle is located to the left of the screen (that means it's a joystick)
    if ((event->getCurrentTarget()->getPositionX()) < (Director::getInstance()->getVisibleSize().width / 2)) {
           
      star->setVisible(true);
      
      _player->move(1);
      
      Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::Move), this, 0.0f, false);
    }
    //checking whether the circle is located on the right side of the screen (that means it's an aim circle)
    else if ((event->getCurrentTarget()->getPositionX()) > (Director::getInstance()->getVisibleSize().width / 2)) {
      //_player->attack(true);
      aim_star->setVisible(true);
      aim_star->setPosition(nodeSpaceLocation);
      
      // left side of the aim circle
      if ((nodeSpaceLocation.x <= event->getCurrentTarget()->getPositionX())) {
        
        Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::incAngle), this, 0.0f, false);

       //Director::getInstance()->getScheduler()->scheduleUpdate();

       /* Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::ChangeCoord), this, 0.0f, false);*/
        //_joystick->schedule((Scene1::ChangeCoord(event)), 0.f, 0, 0.0f, "schedulerKey");
      } 
      //right side of the aim circle
      else if ((nodeSpaceLocation.x > event->getCurrentTarget()->getPositionX())) {
        //LOGI("!!!!!!!!!!!!!!!!!!!!!!!getPositionX = %f, bounds = %f, nodeSpaceLocation.x = %f", event->getCurrentTarget()->getPositionX(), bounds.size.width, nodeSpaceLocation.x);
    
        Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::decAngle), this, 0.0f, false);
      }
      //cube3D->syncPhysicsToNode();
      //_camera->setRotation3D(Vec3(0, (-1)*nodeSpaceLocation.x, 0));
      //auto visibleSize = Director::getInstance()->getVisibleSize();
      //Vec2 origin = Director::getInstance()->getVisibleOrigin();
      
      //_camera->setPosition3D(Vec3(0, _camera->getPosition3D().x + nodeSpaceLocation.x, 0));
      
      //_camera->setPosition3D(_player->_getArmatureDisplay()->getPosition3D()+Vec3(0, 0, 250));
      //_camera->lookAt(_player->_getArmatureDisplay()->getPosition3D(),Vec3(0,1,0));
      
      //_camera->setRotation3D(Vec3(((nodeSpaceLocation.y*0.28) - 14), 0, 0));
    }
  }

  sendUdp();
  
  return true;
}

void Scene1::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Camera* _camera, BillBoard* billboard)
{
  
  auto bounds = event->getCurrentTarget()->getBoundingBox();
  // checking only if the joystick has been released because the stop animation is related to movement
  if (bounds.containsPoint(touch->getLocation())) {
    if ((event->getCurrentTarget()->getPositionX()) < (Director::getInstance()->getVisibleSize().width / 2)) {
      
      star->setVisible(false);
      _player->stop();
      _player->_getArmatureDisplay()->stopAllActions();
      //stops repeated movement from onTouchBegin
      cube3D->stopAllActions();
      //cube3D->syncPhysicsToNode();
      _joystick->stopAllActions();

      Director::getInstance()->getScheduler()->unschedule(
          CC_SCHEDULE_SELECTOR(Scene1::Move), this);
      /*Rect _playerRect = billboard->getBoundingBox();

      AABB _playerAABB = AABB(Vec3(_playerRect.getMinX(), _playerRect.getMinY(), billboard->getPositionZ()),
                              Vec3(_playerRect.getMaxX(), _playerRect.getMaxY(), billboard->getPositionZ()));
      AABB cube3DAABB = cube3D->getAABB();*/
  
      /*if (_playerAABB.intersects(cube3DAABB)) {
        
        cube3D->runAction(MoveBy::create(1, Vec2(0,-90)));
      }*/

     // if (_playerRect.getMinX()<cube3D->getMeshIndexData(0)->getMeshVertexData())

    }
    // release of aim circle
    else if ((event->getCurrentTarget()->getPositionX()) > (Director::getInstance()->getVisibleSize().width / 2)) {
     
      Director::getInstance()->getScheduler()->unschedule(
          CC_SCHEDULE_SELECTOR(Scene1::incAngle), this);

      Director::getInstance()->getScheduler()->unschedule(
          CC_SCHEDULE_SELECTOR(Scene1::decAngle), this);

      aim_star->setVisible(false);
      _player->attack(false);
      _player->_getArmatureDisplay()->stopAllActions();
      _camera->stopAllActions();
      rotationPoint->stopAllActions();
      //stops repeated movement from onTouchBegin
      cube3D->stopAllActions();
      _joystick->stopAllActions();
    }
  }
}

void Scene1::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Camera* _camera, BillBoard* billboard)
{
  auto bounds = event->getCurrentTarget()->getBoundingBox();
  
  location = touch->getLocation();

  // get the location of the touch relative to your button
  nodeSpaceLocation = _node->getParent()->convertToNodeSpace(location);

  int speed = 2;

  //1-L, 2-R, 3-F, 4-B, 13-LF, 14-LB, 23-RF, 24-RB
  if (bounds.containsPoint(touch->getLocation())) {
    //changes applied to the left circle
    if ((event->getCurrentTarget()->getPositionX()) < (Director::getInstance()->getVisibleSize().width / 2)) {
      
      Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::Move), this, 0.0f, false);
    }
    // changes applied to the right circle (aim)
    else if ((event->getCurrentTarget()->getPositionX()) > (Director::getInstance()->getVisibleSize().width / 2)) {
      aim_star->setVisible(true);
      aim_star->setPosition(nodeSpaceLocation);
      _player->attack(false);
      
      //left side of the aim circle
      if ((nodeSpaceLocation.x <= event->getCurrentTarget()->getPositionX())) {
        Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::incAngle), this, 0.0f, false);
      } 
      //right side of the aim circle
      else if ((nodeSpaceLocation.x > event->getCurrentTarget()->getPositionX())) {
        //LOGI("!!!!!!!!!!!!!!!!!!!!!!!getPositionX = %f, bounds = %f, nodeSpaceLocation.x = %f", event->getCurrentTarget()->getPositionX(), bounds.size.width, nodeSpaceLocation.x);
        star->setVisible(false);
        Director::getInstance()->getScheduler()->schedule(
          CC_SCHEDULE_SELECTOR(Scene1::decAngle), this, 0.0f, false);
      }
    }
  }
}

void Scene1::messageReceived(Event* event)
{	
  /*projectile2 = cocos2d::Sprite::create("projectile.png");
  addChild(projectile2);
  auto _player = event->getCurrentTarget();
  projectile2->setPosition(_player->getPosition());
  auto actionMove1 = MoveTo::create(2.0f, g_engine.gotDest);
  auto actionRemove1 = RemoveSelf::create();
  projectile2->runAction(Sequence::create(actionMove1,actionRemove1, nullptr));*/
}

void Scene1::menuCloseCallback(Ref* pSender, Anim* _player, Layer* world, Layer* hudlayer)
{
	//show fullscreen interstitial
  //AdmobHelper::showfullscreenAd();
  //CCLayer::onEnterTransitionDidFinish();
  AdmobHelper::hideAd();
  _player->~Anim();
  g_engine.LeaveGame();
  auto scene = Scene0::createScene();
  Director::getInstance()->replaceScene(scene);
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

void Scene1::basicShader(cocos2d::Sprite3D * sprite3d)          //////WIP
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

void Scene1::aim()
{
    //adding camera for closer aim
  Size s = Director::getInstance()->getVisibleSize();
  auto camera =Camera::createPerspective(60, s.width/s.height, 0.1f, 200);
  
  // set parameters for camera
  camera->setPosition3D(Vec3(0, 100, 100));
  camera->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
  
  // hiding player sprite if camera is enabled
  camera->setCameraFlag(CameraFlag::USER1);
 // _player->_getArmatureDisplay()->setCameraMask(CameraFlag::USER1);
  addChild(camera); //add camera to the scene
}

void Scene1::incAngle(float dt) {
  if (angle < 360) {
    angle += 1.2f;
  }
  else {
    angle = 1.2f;
  }
  rotationPoint->setRotation3D(rotationPoint->getRotation3D() + Vec3(0,(angle/100),0));
  box->setRotation3D(box->getRotation3D() + Vec3(0,(angle/100),0));
}

void Scene1::decAngle(float dt) {
  if (angle > 0) {
      angle -= 1.2f;
  }
  else {
    angle = 358.8f;
  }
  rotationPoint->setRotation3D(rotationPoint->getRotation3D() - Vec3(0,(angle/100),0));
  box->setRotation3D(box->getRotation3D() - Vec3(0,(angle/100),0));
}

void Scene1::Move(float dt) {
  int speed = 6;
  star->setPosition(nodeSpaceLocation); 
  //1-L, 2-R, 3-F, 4-B, 13-LF, 14-LB, 23-RF, 24-RB
  if ((nodeSpaceLocation.x < (coord.x -(0.3*(coord.x - Left.x)))) && (nodeSpaceLocation.y < (coord.y -(0.3*(coord.y - Down.y))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) - Vec3((speed),0,0)));
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) - Vec3(0,0,(speed))));
    move_state = 24;
  }
  else if ((nodeSpaceLocation.x < (coord.x -(0.3*(coord.x - Left.x)))) && (nodeSpaceLocation.y > (coord.y +(0.3*(Up.y - coord.y))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3((speed),0,0)));
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) - Vec3(0,0,(speed))));
    move_state = 14;
  }
  else if ((nodeSpaceLocation.x > (coord.x +(0.3*(Right.x - coord.x)))) && (nodeSpaceLocation.y < (coord.y -(0.3*(coord.y - Down.y))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) - Vec3((speed),0,0)));
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(0,0,(speed))));
    move_state = 23;
  }
  else if ((nodeSpaceLocation.x > (coord.x +(0.3*(Right.x - coord.x)))) && (nodeSpaceLocation.y > (coord.y +(0.3*(Up.y - coord.y))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3((speed),0,0)));
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(0,0,(speed))));
    move_state = 13;
  }
  else if ((nodeSpaceLocation.x < (coord.x -(0.3*(coord.x - Left.x)))) && (nodeSpaceLocation.y < (coord.y +(/*0.3*/0.3*(Up.y - coord.y)))) && (nodeSpaceLocation.y > (coord.y -(/*0.3*/0.3*(coord.y - Down.y))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) - Vec3(0,0,(speed))));
    move_state = 4;
  }
  else if ((nodeSpaceLocation.x > (coord.x +(0.3*(Right.x - coord.x)))) && (nodeSpaceLocation.y < (coord.y +(/*0.3*/0.3*(Up.y - coord.y)))) && (nodeSpaceLocation.y > (coord.y -(/*0.3*/0.3*(coord.y - Down.y))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3(0,0,(speed))));
    move_state = 3;
  }
  else if ((nodeSpaceLocation.y < (coord.y -(0.3*(coord.y - Down.y)))) && (nodeSpaceLocation.x < (coord.x +(/*0.3*/0.3*(Right.x - coord.x)))) && (nodeSpaceLocation.x > (coord.x -(/*0.3*/0.3*(coord.x - Left.x))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) - Vec3((speed),0,0)));
    move_state = 2;
  }
  else if ((nodeSpaceLocation.y > (coord.y +(0.3*(Up.y - coord.y)))) && (nodeSpaceLocation.x < (coord.x +(/*0.3*/0.3*(Right.x - coord.x)))) && (nodeSpaceLocation.x > (coord.x -(/*0.3*/0.3*(coord.x - Left.x))))) {
    cube3D->setPosition3D(convertToNodeSpace3D(cube3D, convertToWorldSpace3D(cube3D, cube3D->getPosition3D()) + Vec3((speed),0,0)));
    move_state = 1;
  }
  else if ((nodeSpaceLocation.x < (coord.x +(/*0.3*/0.3*(Right.x - coord.x)))) && (nodeSpaceLocation.x > (coord.x -(/*0.3*/0.3*(coord.x - Left.x)))) && (nodeSpaceLocation.y < (coord.y +(/*0.3*/0.3*(Up.y - coord.y)))) && (nodeSpaceLocation.y > (coord.y -(/*0.3*/0.3*(coord.y - Down.y))))) {
    // center of the joystick
    move_state = 0;
  }
  //LOGI("!!!!!!!!!!!!!!!!!!!!!!!move_state = %d", move_state);
}

void Scene1::sendUdp() {
  //Sending udp about player's location
  
  

  //How to send udp message with projectile info
  /*projectile = cocos2d::Sprite::create("projectile.png");
  addChild(projectile);
  std::stringstream ss;
  // 1  - Just an example for how to get the  _player object
  auto _player = event->getCurrentTarget();
 
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
}

Vec3 Scene1::convertToWorldSpace3D(Node* container, Vec3 position) {
    Vec3 result = position;
    Mat4 mat = getNodeToWorldSpaceMatrix(container);
    mat.transformPoint(&result);
    return result;
  }
  
  Vec3 Scene1::convertToNodeSpace3D(Node* container, Vec3 position) {
    Vec3 result = position;
    Mat4 mat = getNodeToWorldSpaceMatrix(container);
    mat.inverse();
    mat.transformPoint(&result);
    return result;
  }
  
  Mat4 Scene1::getNodeToWorldSpaceMatrix(Node* node) {
    Mat4 matrix;
    matrix.translate(node->getPositionX(), node->getPositionY(), node->getPositionZ());
    auto quat = node->getRotationQuat();
    matrix.rotate(quat);
    matrix.scale(node->getScaleX(), node->getScaleY(), node->getScaleZ());
    
  if(node->getParent() != nullptr){
    Mat4 parentMatrix = Scene1::getNodeToWorldSpaceMatrix(node->getParent());
    parentMatrix.multiply(matrix);
    matrix = parentMatrix;
  }
  return matrix;
}