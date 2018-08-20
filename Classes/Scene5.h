#ifndef __SCENE5_H__
#define __SCENE5_H__

#include "cocos2d.h"

class Scene5 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Scene5);
};

#endif // __SCENE5_H__
