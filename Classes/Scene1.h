#ifndef __SCENE1_H__
#define __SCENE1_H__

#include "cocos2d.h"
#include "Anim.h"
#include "physics3d/CCPhysics3D.h"

class Scene1 : public cocos2d::Layer
{
public:
    //cocos2d::Sprite3D *cube3D;
    
    cocos2d::RenderTexture *renderTexDefault;
    cocos2d::RenderTexture *renderTexWithBuffer;
    
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender, Anim* _player, Layer* world, Layer* hudlayer);
    void messageReceived(cocos2d::Event* event);
	// implement the "static create()" method manually
	CREATE_FUNC(Scene1);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Camera* _camera, BillBoard* billboard);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Camera* _camera, BillBoard* billboard);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event, Anim* _player, Sprite* _node, Camera* _camera, BillBoard* billboard);
	//void onTouchCancelled();
	//cocos2d::Sprite* projectile;
    //cocos2d::Sprite* projectile2;
	void convertToUint8 (float f, std::vector<unsigned char>& v);
	void basicShader(cocos2d::Sprite3D * sprite3d);
	void aim();
	void incAngle(float dt);
	void decAngle(float dt);
	void sendUdp();

    //1-L, 2-R, 3-F, 4-B, 13-LF, 14-LB, 23-RF, 24-RB
	int move_state;
	void Move(float dt);

	Vec3 convertToWorldSpace3D(Node* container, Vec3 position);
	Vec3 convertToNodeSpace3D(Node* container, Vec3 position);
	Mat4 getNodeToWorldSpaceMatrix(Node* node);
private:
	//location of touch relative to screen
	Vec2 location;
	//location of touch relative to sprite
	Vec2 nodeSpaceLocation;
	//level map
	Sprite3D* cube3D;
	//player's rotation point, to always rotate map around where player's location is
	Node* rotationPoint;
	//joystick coordinates for further modification
	Vec3 coord; 
	//map rotation angle
	float angle;
	//directional buttons' coordinates
	Vec2 Up;
	Vec2 Left;
    Vec2 Right;
    Vec2 Down;
    
    //skybox
    Sprite3D* box;

	Sprite* _joystick;
	Sprite* _aim;
	Layer* world;
	Layer* hudlayer;
	Sprite* star;
	Sprite* aim_star;
	cocos2d::Label* labelTouchInfo;
	const static int MAX_TOUCHES = 5;
};

#endif // __SCENE1_H__
