#include "Anim.h"

Anim::Anim(cocos2d::Size winSize) :
    _isJumpingA(false),
    _isJumpingB(false),
    _isSquating(false),
    _isAttackingA(false),
    _isAttackingB(false),
    _skinIndex(0),
    _weaponRIndex(0),
    _weaponLIndex(0),
    _faceDir(1),
    _aimDir(0),
    _moveDir(0),
    _aimRadian(0.0f),
    _speedX(0.0f),
    _speedY(0.0f),
    _armatureDisplay(nullptr),
    _weaponR(nullptr),
    _weaponL(nullptr),
    _aimState(nullptr),
    _walkState(nullptr),
    _attackState(nullptr),
    _target()
{
    _armatureDisplay = dragonBones::CCFactory::getFactory()->buildArmatureDisplay("mecha_1502b");
    _armatureDisplay->setPosition(cocos2d::Vec2(winSize.width * 0.5, winSize.height * 0.2));
    _armatureDisplay->setScale(0.5f);
    _armatureDisplay->getEventDispatcher()->setEnabled(true);
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_IN_COMPLETE, std::bind(&Anim::_animationEventHandler, this, std::placeholders::_1));
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FADE_OUT_COMPLETE, std::bind(&Anim::_animationEventHandler, this, std::placeholders::_1));
    _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&Anim::_animationEventHandler, this, std::placeholders::_1));

    _weaponR = _armatureDisplay->getArmature()->getSlot("weapon_r")->getChildArmature();
    _weaponL = _armatureDisplay->getArmature()->getSlot("weapon_l")->getChildArmature();
    const auto weaponRDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponR->getDisplay());
    const auto weaponLDisplay = static_cast<dragonBones::CCArmatureDisplay*>(_weaponL->getDisplay());
    weaponRDisplay->getEventDispatcher()->setEnabled(true);
    weaponLDisplay->getEventDispatcher()->setEnabled(true);
    weaponRDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Anim::_frameEventHandler, this, std::placeholders::_1));
    weaponLDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Anim::_frameEventHandler, this, std::placeholders::_1));
    
    _armatureDisplay->getAnimation()->fadeIn(
        "idle", -1.0f, -1, 
        0, NORMAL_ANIMATION_GROUP
    )->resetToPose = false;
    _walkState = nullptr;
    
    /*_attackState = _armatureDisplay->getAnimation()->fadeIn(
        "attack_01", -1.0f, -1,
        0, ATTACK_ANIMATION_GROUP
    );
    _attackState->resetToPose = false;

    _moveDir = 1;
    _walkState = _armatureDisplay->getAnimation()->fadeIn(
                "walk", -1.0f, -1,
                0, NORMAL_ANIMATION_GROUP
            );
    _walkState->resetToPose = false;*/

    _updateAnimation();
}

const std::string Anim::NORMAL_ANIMATION_GROUP = "normal";
const std::string Anim::ATTACK_ANIMATION_GROUP = "attack";
const float Anim::NORMALIZE_MOVE_SPEED = 3.6f;

Anim::~Anim()
{
} 

void Anim::_animationEventHandler(cocos2d::EventCustom * event)
{
    const auto eventObject = (dragonBones::EventObject*)event->getUserData();
    _isJumpingB = true;
}

void Anim::_frameEventHandler(cocos2d::EventCustom* event)
{
    const auto eventObject = static_cast<dragonBones::EventObject*>(event->getUserData());
    if (eventObject->name == "fire")
    {
        const auto display = static_cast<dragonBones::CCArmatureDisplay*>(eventObject->armature->getDisplay());

        const auto transform = display->getNodeToWorldTransform();
        cocos2d::Vec3 localPoint(eventObject->bone->global.x, eventObject->bone->global.y, 0.0f);
        cocos2d::Vec2 globalPoint;
        transform.transformPoint(&localPoint);
       // globalPoint.set(localPoint.x - CoreElementGame::instance->getPosition().x, localPoint.y - CoreElementGame::instance->getPosition().y);

        //_fire(globalPoint);
    }
}

void Anim::move(int dir)
{
  _moveDir = dir;
  _walkState = _armatureDisplay->getAnimation()->fadeIn(
              "walk", -1.0f, -1,
              0, NORMAL_ANIMATION_GROUP
          );
  _walkState->resetToPose = false;
}

void Anim::stop()
{
    _armatureDisplay->getAnimation()->fadeIn(
            "idle", -1.0f, -1, 
            0, NORMAL_ANIMATION_GROUP
        )->resetToPose = false;
        _walkState = nullptr;
}

void Anim::jump()
{
    if (_isJumpingA)
    {
        return;
    }

    _isJumpingA = true;
    _armatureDisplay->getAnimation()->fadeIn(
        "jump_1", -1.0f, -1, 
        0, NORMAL_ANIMATION_GROUP
    )->resetToPose = false;

    _walkState = nullptr;
}

void Anim::attack(bool isAttacking)
{
   /* if (_isAttackingA == isAttacking)
    {
        return;
    }

    _isAttackingA = isAttacking; */
    if (isAttacking == true) {
      _attackState = _armatureDisplay->getAnimation()->fadeIn(
          "attack_01", -1.0f, -1,
          0, ATTACK_ANIMATION_GROUP
      );
      _attackState->resetToPose = false;
    }
    else {
      ////////////////////////////////////////////////////////////////WIP
      _attackState = nullptr;
    }
}

void Anim::_updateAnimation()
{

}

dragonBones::CCArmatureDisplay* Anim::_getArmatureDisplay()
{
  return _armatureDisplay;
}