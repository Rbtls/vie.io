#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

USING_NS_CC;

class Anim
{

  private:
    bool _isJumpingA;
    bool _isJumpingB;
    bool _isSquating;
    bool _isAttackingA;
    bool _isAttackingB;
    unsigned _weaponRIndex;
    unsigned _weaponLIndex;
    unsigned _skinIndex;
    int _faceDir;
    int _aimDir;
    int _moveDir;
    float _aimRadian;
    float _speedX;
    float _speedY;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    dragonBones::Armature* _weaponR;
    dragonBones::Armature* _weaponL;
    dragonBones::AnimationState* _aimState;
    dragonBones::AnimationState* _walkState;
    dragonBones::AnimationState* _attackState;
    cocos2d::Vec2 _target;

    void _animationEventHandler(cocos2d::EventCustom* event);
    void _frameEventHandler(cocos2d::EventCustom* event);
  public:
  	void move(int dir);
  	void stop();
  	void attack(bool isAttacking);
  	static const std::string NORMAL_ANIMATION_GROUP;
  	static const std::string ATTACK_ANIMATION_GROUP;
  	static const float NORMALIZE_MOVE_SPEED;
    Anim(cocos2d::Size);
    ~Anim();
    void jump();
    void _updateAnimation();
    dragonBones::CCArmatureDisplay* _getArmatureDisplay();
};
