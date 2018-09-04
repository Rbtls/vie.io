#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "cocos2d.h"

class Scene2 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Scene2);
};

#endif // __SCENE2_H__