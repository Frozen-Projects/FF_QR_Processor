#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, Category = "Frozen Forest|QR|Nayuki")
enum class ENayukiQrTolerance : uint8
{
	Quartile	UMETA(DisplayName = "Quartile"),
	Low			UMETA(DisplayName = "Low"),
	Mid			UMETA(DisplayName = "Mid"),
	High		UMETA(DisplayName = "High"),
};
ENUM_CLASS_FLAGS(ENayukiQrTolerance)

UENUM(BlueprintType, Category = "Frozen Forest|QR|ZXing")
enum class EZXingFormat : uint8
{
    None = 0,           ///< Used as a return value if no valid barcode has been detected
    Aztec,              ///< Aztec
    Codabar,            ///< Codabar
    Code39,             ///< Code39
    Code93,             ///< Code93
    Code128,            ///< Code128
    DataBar,            ///< GS1 DataBar, formerly known as RSS 14
    DataBarExpanded,    ///< GS1 DataBar Expanded, formerly known as RSS EXPANDED
    DataMatrix,         ///< DataMatrix
    EAN8,               ///< EAN-8
    EAN13,              ///< EAN-13
    ITF,                ///< ITF (Interleaved Two of Five)
    MaxiCode,           ///< MaxiCode
    PDF417,             ///< PDF417
    QRCode,             ///< QR Code
    UPCA,               ///< UPC-A
    UPCE,               ///< UPC-E
    MicroQRCode,        ///< Micro QR Code
};
ENUM_CLASS_FLAGS(EZXingFormat)