LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/hellocpp/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/core/custom/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/core/sqlite/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/core/db/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/core/web/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/core/tcp/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/core/CSVparser/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/data/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/gateWay/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/manor/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/crusade/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/popUp/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/utils/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/loading/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/guide/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/battle/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/notice/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/sound/*.cpp)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_SHARED_LIBRARIES := mobclickcpp_shared

include $(BUILD_SHARED_LIBRARY)


$(call import-module,./prebuilt-mk)
$(call import-module,libmobclickcpp)
