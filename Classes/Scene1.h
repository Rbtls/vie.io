#ifndef __SCENE1_H__
#define __SCENE1_H__

#include "cocos2d.h"
#include "Anim.h"

class Scene1 : public cocos2d::Layer
{
public:
    cocos2d::Sprite3D *cube3D;
    
    cocos2d::RenderTexture *renderTexDefault;
    cocos2d::RenderTexture *renderTexWithBuffer;

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
    void messageReceived(cocos2d::Event* unused_event);
	// implement the "static create()" method manually
	CREATE_FUNC(Scene1);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event, Anim* _player, Sprite* _node, Sprite3D* _map, Camera* _camera, Skybox* _box);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Sprite3D* _map);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Sprite3D* _map, Skybox* _box);
	//void onTouchCancelled();
	//cocos2d::Sprite* projectile;
    //cocos2d::Sprite* projectile2;
	void convertToUint8 (float f, std::vector<unsigned char>& v);
	void basicShader(cocos2d::Sprite3D * sprite3d);
	void aim();
    
    //1-L, 2-R, 3-F, 4-B, 13-LF, 14-LB, 23-RF, 24-RB
	int move_state;
private:
	cocos2d::Label* labelTouchInfo;
	const static int MAX_TOUCHES = 5;
};

#endif // __SCENE1_H__
