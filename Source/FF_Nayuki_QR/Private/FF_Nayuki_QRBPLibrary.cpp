// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_Nayuki_QRBPLibrary.h"
#include "FF_Nayuki_QR.h"

THIRD_PARTY_INCLUDES_START
#include "qrcodegen.hpp"                            // https://github.com/nayuki/QR-Code-generator
THIRD_PARTY_INCLUDES_END

UFF_Nayuki_QRBPLibrary::UFF_Nayuki_QRBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UFF_Nayuki_QRBPLibrary::GenerateQRCode(UTexture2D*& OutQRCode, const FString QR_String, EQrErrorTolerance ErrorTolerance)
{
    if (QR_String.IsEmpty())
    {
        return false;
    }

    qrcodegen::QrCode::Ecc ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;

    switch (ErrorTolerance)
    {
    case EQrErrorTolerance::Quartile:
        ToleranceLevel = qrcodegen::QrCode::Ecc::QUARTILE;
        break;
    case EQrErrorTolerance::Low:
        ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;
        break;
    case EQrErrorTolerance::Mid:
        ToleranceLevel = qrcodegen::QrCode::Ecc::MEDIUM;
        break;
    case EQrErrorTolerance::High:
        ToleranceLevel = qrcodegen::QrCode::Ecc::HIGH;
        break;
    default:
        break;
    }

    qrcodegen::QrCode QRCode = qrcodegen::QrCode::encodeText(TCHAR_TO_UTF8(*QR_String), ToleranceLevel);

    uint8 QRSize = QRCode.getSize();

    FColor Black = FColor::Black;
    FColor White = FColor::White;
    TArray<FColor> QRPixels;

    QRPixels.SetNumZeroed(QRSize * QRSize);

    for (uint8 x = 0; x < QRSize; x++)
    {
        for (uint8 y = 0; y < QRSize; y++)
        {
            FColor color = QRCode.getModule(x, y) ? White : Black;
            QRPixels[x + y * QRSize] = color;
        }
    }

    UTexture2D* QRTexture = UTexture2D::CreateTransient(QRSize, QRSize, EPixelFormat::PF_B8G8R8A8, "QRCode");
    void* data = QRTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_ONLY);
    FMemory::Memcpy(data, QRPixels.GetData(), QRSize * QRSize * 4);
    QRTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
    QRTexture->UpdateResource();
    QRTexture->Filter = TextureFilter::TF_Nearest;

    if (IsValid(QRTexture))
    {
        OutQRCode = QRTexture;
        return true;
    }

    else
    {
        return false;
    }
}