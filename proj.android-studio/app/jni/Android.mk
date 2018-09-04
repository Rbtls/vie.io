LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
IMPORT_PATH := $(LOCAL_PATH)/../../../../..

$(call import-add-path,$(LOCAL_PATH))
$(call import-add-path,$(LOCAL_PATH)/../../../Classes)
$(call import-add-path,$(LOCAL_PATH)/../../../Classes/dragonBones)
$(call import-add-path,$(IMPORT_PATH)/cocos2d-x-3.15)
$(call import-add-path,$(IMPORT_PATH)/cocos2d-x-3.15/cocos)
$(call import-add-path,$(IMPORT_PATH)/cocos2d-x-3.15/external)
$(call import-add-path,$(IMPORT_PATH)/cocos2d-x-3.15/extensions)
$(call import-add-path,$(IMPORT_PATH)/cocos2d-x-3.15/cocos/platform/android)
$(call import-add-path,$(IMPORT_PATH)/cocos2d-x-3.15/cocos/audio/include)
$(call import-add-path,$(IMPORT_PATH)/Android)
$(call import-add-path,$(IMPORT_PATH)/android-ndk-r14b/sources/android)
$(call import-add-path,$(IMPORT_PATH)/android-ndk-r14b/sources/android/ndk_helper)
$(call import-add-path,$(IMPORT_PATH)/android-ndk-r14b/sources/gpg-cpp-sdk/android/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/hellocpp/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/animation/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/armature/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/cocos2dx/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/core/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/event/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/factory/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/geom/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/model/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/dragonBones/parser/*.cpp)
MY_CPP_LIST += $(wildcard $(IMPORT_PATH)/cocos2d-x-3.15/cocos/*.cpp)
MY_CPP_LIST := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(MY_CPP_LIST)

LOCAL_JAVA_RESOURCE_DIRS := $(IMPORT_PATH)/cocos2d-x-3.15/plugin/protocols/proj.android/src/org/cocos2dx/plugin/

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes/dragonBones
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/cocos
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/cocos/base
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/cocos/platform
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/extensions
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/cocos/platform/android
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/cocos/include
LOCAL_C_INCLUDES += $(IMPORT_PATH)/android-ndk-r14b/sources/gpg-cpp-sdk/android/include
LOCAL_C_INCLUDES += $(shell find $(LOCAL_PATH)/../../../Classes -type d)
LOCAL_C_INCLUDES += $(shell find $(LOCAL_PATH)/../../../Classes/dragonBones -type d)
LOCAL_C_INCLUDES += $(IMPORT_PATH)/cocos2d-x-3.15/extensions

LOCAL_LDFLAGS := -Wl,-allow-multiple-definition

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_internal_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cpufeatures 
LOCAL_WHOLE_STATIC_LIBRARIES += ndk_helper 
LOCAL_WHOLE_STATIC_LIBRARIES += gpg-1 
LOCAL_WHOLE_STATIC_LIBRARIES += PluginProtocolStatic


# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

LOCAL_STATIC_JAVA_AAR_LIBRARIES := play-services-ads 
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-ads-lite 
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-tasks
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-base 
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-basement
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-gass
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-clearcut
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-drive
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-plus
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-games
LOCAL_STATIC_JAVA_AAR_LIBRARIES += play-services-nearby

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,extensions)
$(call import-module,ndk_helper)
$(call import-module,android/cpufeatures)
$(call import-module,dragonBones/proj.android-studio)
$(call import-module,gpg-cpp-sdk/android)
$(call import-add-path,$(LOCAL_PATH))
$(call import-add-path,$(LOCAL_PATH)/../../../../AndroidStudioProjects/vie.io/Classes/dragonBones/proj.android-studio)
$(call import-module,external/curl/prebuilt/android)
$(call import-module,plugin/protocols/proj.android/jni)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
