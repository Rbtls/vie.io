#ifndef __SCENE0_H__
#define __SCENE0_H__

#include "cocos2d.h"


class Scene0 : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Scene0);

	void Play(Ref *pSender);
	void CreateGame(Ref *pSender);
	void Settings(Ref *pSender);
	void HowTo(Ref *pSender);
	void Quit(Ref *pSender);
};

#endif // __SCENE0_H__
