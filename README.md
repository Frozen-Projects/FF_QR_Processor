# FF_QR_Processor
 
## THIRD PARTY LIBRARIES
https://github.com/nayuki/QR-Code-generator

https://github.com/zxing-cpp/zxing-cpp

## INSTRUCTIONS FOR OPENCV
* Unzip ``Source/OpenCv_Libs/Win64/lib/opencv_world490d.zip`` before starting your IDE. We do this because GitHub doesn't allow us to upload files bigger than 100MB without LFS.

## INSTRUCTIONS FOR ZXING
* Open CMake and create solution files. (this will create Version.h) but don't start Visual Studio from .sln
* Copy ``zxing-cpp/build/core/Version.h`` to ``builder/core/src``
* Copy all ``zxing-cpp/core/src`` to ``buider/core/src``
* Fix Android NDK location in ``AndroidBuild.bat`` if necessary.
* Start terminal and run ``AndroidBuild.bat``