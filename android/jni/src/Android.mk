LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SRC_PATH := ../../../src

LOCAL_CFLAGS += -g -std=c99

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

YOUR_SRC_FILES= $(SRC_PATH)/core/game.c $(SRC_PATH)/core/node.c $(SRC_PATH)/gui/hashi_gui.c $(SRC_PATH)/gui/model.c  $(SRC_PATH)/tools/file.c $(SRC_PATH)/tools/generate.c $(SRC_PATH)/solver/solver.c

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c $(YOUR_SRC_FILES)

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_image

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
