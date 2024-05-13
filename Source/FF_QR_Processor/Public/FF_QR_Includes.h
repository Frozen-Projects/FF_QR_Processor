#pragma once

// External Includes.
#include "ExtendedVarsBPLibrary.h"

// Custom Includes.
#include "FF_QR_Enums.h"

// UE Includes
#include "ImageUtils.h"                         // Resize Nayuki QR

THIRD_PARTY_INCLUDES_START
// Nayuki QR.
#include "qrcodegen.hpp"                        // https://github.com/nayuki/QR-Code-generator

// ZXing.
#include "ReadBarcode.h"
#include "GTIN.h"
#include "MultiFormatWriter.h"
#include "BitMatrix.h"

// OpenCV.
#include <opencv2/objdetect.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn/dnn.hpp"
#include "opencv2/opencv.hpp"
THIRD_PARTY_INCLUDES_END