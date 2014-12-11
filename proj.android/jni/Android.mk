LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
   ../../Classes/AppDelegate.cpp \
                   ../../Classes/StartScene.cpp \
                    ../../Classes/StoryScene.cpp \
                    ../../Classes/BattleMapScene.cpp \
                    ../../Classes/BattleCharacter.cpp \
                     ../../Classes/BattleAI.cpp \
                     ../../Classes/slg_tools.cpp \
                      ../../Classes/DuelScene.cpp \
                       ../../Classes/AnimationManager.cpp \
                       ../../Classes/GameUI.cpp \
                         ../../Classes/RangeObject.cpp \
                         ../../Classes/BattleManager.cpp \
                          ../../Classes/BattleObject.cpp \
                         ../../Classes/DataLoader.cpp \
                           ../../Classes/PlayerDataManager.cpp \
                            ../../Classes/EventHandler.cpp \



LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += \
$(LOCAL_PATH)/../../../iconv/include \
$(LOCAL_PATH)/../../../iconv/libcharset \
$(LOCAL_PATH)/../../../iconv/lib \
$(LOCAL_PATH)/../../../iconv/libcharset\include \
$(LOCAL_PATH)/../../../iconv/srclib \
$(LOCAL_PATH)/../../../iconv

LOCAL_STATIC_LIBRARIES := curl_static_prebuilt

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += jsoncpp_static
include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,jsoncpp)