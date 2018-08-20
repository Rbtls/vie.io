#ifndef __SCENE1_H__
#define __SCENE1_H__

#include "cocos2d.h"

class Scene1 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
    void messageReceived(cocos2d::Event* unused_event);
	// implement the "static create()" method manually
	CREATE_FUNC(Scene1);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
	//cocos2d::Sprite* projectile;
    //cocos2d::Sprite* projectile2;
	void convertToUint8 (float f, std::vector<unsigned char>& v);
private:
	cocos2d::Label* labelTouchInfo;
};

#endif // __SCENE1_H__
