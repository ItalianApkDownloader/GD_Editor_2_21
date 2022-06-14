APP_ABI := armeabi-v7a
APP_CFLAGS := -fPIC -shared -fexceptions
APP_PLATFORM := android-17
APP_STL:= gnustl_static
APP_CPPFLAGS += -std=c++14 -fexceptions
LOCAL_CPP_FEATURES := rtti
NDK_TOOLCHAIN_VERSION := clang
LOCAL_CPPFLAGS := -fexceptions