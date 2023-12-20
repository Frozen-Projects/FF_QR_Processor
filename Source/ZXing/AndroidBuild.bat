Echo Off
Set Arg1=%1

If not exist C:\Users\Administrator\AppData\Local\Android\Sdk\ndk\21.4.7075529\ndk-build.cmd Goto NDK_NOT_FOUND

Set NDK_BUILD_PATH="C:\Users\Administrator\AppData\Local\Android\Sdk\ndk\21.4.7075529\ndk-build.cmd"

If "%Arg1%"=="clean" Goto START_CLEAN

:START_BUILD
%NDK_BUILD_PATH% APP_BUILD_SCRIPT=android/Android.mk NDK_PROJECT_PATH=core/src NDK_APPLICATION_MK=android/Application.mk NDK_OUT=lib/Android
Exit

:START_CLEAN
%NDK_BUILD_PATH% APP_BUILD_SCRIPT=android/Android.mk NDK_PROJECT_PATH=core/src NDK_APPLICATION_MK=android/Application.mk NDK_OUT=lib/Android clean
Exit

:NDK_NOT_FOUND
Echo "NDK not found"