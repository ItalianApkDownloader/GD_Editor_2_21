# GDPS Editor 2.2

The *entire* client source code of the GDPS, including the editor code. 
Please do not use for malicious purposes 💛

## Code highlights

- [Entry Point](https://github.com/ItalianApkDownloader/GD_Editor_2_21/blob/master/src/main.cpp#L1399)

- [Editor](https://github.com/ItalianApkDownloader/GD_Editor_2_21/blob/master/src/hooks/LevelEditorLayerExt.cpp)
- [Two player platformer](https://github.com/ItalianApkDownloader/GD_Editor_2_21/blob/master/src/hooks/DPADControl.cpp)
- [Arrow trigger fix](https://github.com/ItalianApkDownloader/GD_Editor_2_21/blob/master/src/hooks/CollisionFix.cpp)

## Warning

Current state of the project
 - unmaintained
 - unorganized
 - bad code
 - bugs
 - undefined behaviour

If you are not familiar with C/C++, function hooking, and similar concepts, we recommend that you avoid using this project. Also, please refrain from copying any part of it for your future projects.

## Building / Using

Requires [NDK r16b](https://github.com/android/ndk/wiki/Unsupported-Downloads#r16b) and GD Subzero 2.2.12 released on 11 July 2022.

- on an empty folder (with the desired project name), clone this repository into a folder called exactly `jni`

- use the following command to build (edit the paths) `ndk-build NDK_PROJECT_PATH=C:\example\GD_Editor_2_21 NDK_APPLICATION_MK=C:\example\GD_Editor_2_21\jni\Application.mk MAIN_LOCAL_PATH=C:\example\GD_Editor_2_21\jni`

- edit the smali of the APK to include the shared libraries and add them to lib\armeabi-v7a folder (delete all other architectures)
  
- optional: edit apk name/package, change servers url.

