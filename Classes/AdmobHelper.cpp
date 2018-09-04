/*
Copyright (c) 2014 Mudafar
GPLv3
*/


#include "AdmobHelper.h"
#include "cocos2d.h"


bool AdmobHelper::isAdShowing = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>

const char* CurrentActivity = "org/cocos2dx/cpp/AppActivity";
//const char* AppActivityClassName = "org/cocos2dx/cpp/FirstActivity";
//const char* SecondActivity = "org/cocos2dx/cpp/SecondActivity";

void AdmobHelper::hideAd()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CurrentActivity, "hideAdmobBannerAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = false;
	}
}

void AdmobHelper::showAd()
{

	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CurrentActivity, "showAdmobBannerAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}

}

    void AdmobHelper::showfullscreenAd()
{
    CCLOG(" Android : showInterstitial() called");
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CurrentActivity, "showInterstitial", "()V"))
    { 
    t.env->CallStaticVoidMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
    }
}

#else


void AdmobHelper::hideAd()
{
	CCLOG("hideAd() called");
	isAdShowing = false;
	return; //nothing
}


void AdmobHelper::showAd()
{
	CCLOG("showAd() called");
	isAdShowing = true;
	return; //nothing;

}

#endif
