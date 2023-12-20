// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_Nayuki_QRBPLibrary.h"
#include "FF_Nayuki_QR.h"

UFF_Nayuki_QRBPLibrary::UFF_Nayuki_QRBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UFF_Nayuki_QRBPLibrary::NayukiQr_GenerateQRCode(UTexture2D*& OutQRCode, const FString QR_String, ENayukiQrTolerance ErrorTolerance)
{
    if (QR_String.IsEmpty())
    {
        return false;
    }

    qrcodegen::QrCode::Ecc ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;

    switch (ErrorTolerance)
    {
    case ENayukiQrTolerance::Quartile:
        ToleranceLevel = qrcodegen::QrCode::Ecc::QUARTILE;
        break;
    case ENayukiQrTolerance::Low:
        ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;
        break;
    case ENayukiQrTolerance::Mid:
        ToleranceLevel = qrcodegen::QrCode::Ecc::MEDIUM;
        break;
    case ENayukiQrTolerance::High:
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

EZXingFormat UFF_Nayuki_QRBPLibrary::ConvertToBlueprintFormat(ZXing::BarcodeFormat Format)
{
    switch (Format)
    {
    case ZXing::BarcodeFormat::None:
        return EZXingFormat::None;
    case ZXing::BarcodeFormat::Aztec:
        return EZXingFormat::Aztec;
    case ZXing::BarcodeFormat::Codabar:
        return EZXingFormat::Codabar;
    case ZXing::BarcodeFormat::Code39:
        return EZXingFormat::Code39;
    case ZXing::BarcodeFormat::Code93:
        return EZXingFormat::Code93;
    case ZXing::BarcodeFormat::Code128:
        return EZXingFormat::Code128;
    case ZXing::BarcodeFormat::DataBar:
        return EZXingFormat::DataBar;
    case ZXing::BarcodeFormat::DataBarExpanded:
        return EZXingFormat::DataBarExpanded;
    case ZXing::BarcodeFormat::DataMatrix:
        return EZXingFormat::DataMatrix;
    case ZXing::BarcodeFormat::EAN8:
        return EZXingFormat::EAN8;
    case ZXing::BarcodeFormat::EAN13:
        return EZXingFormat::EAN13;
    case ZXing::BarcodeFormat::ITF:
        return EZXingFormat::ITF;
    case ZXing::BarcodeFormat::MaxiCode:
        return EZXingFormat::MaxiCode;
    case ZXing::BarcodeFormat::PDF417:
        return EZXingFormat::PDF417;
    case ZXing::BarcodeFormat::QRCode:
        return EZXingFormat::QRCode;
    case ZXing::BarcodeFormat::UPCA:
        return EZXingFormat::UPCA;
    case ZXing::BarcodeFormat::UPCE:
        return EZXingFormat::UPCE;
    case ZXing::BarcodeFormat::MicroQRCode:
        return EZXingFormat::MicroQRCode;
    default:
        return EZXingFormat::None;
    }
}

ZXing::BarcodeFormat UFF_Nayuki_QRBPLibrary::ConvertToBarcodeFormat(EZXingFormat Format)
{
    switch (Format)
    {
    case EZXingFormat::None:
        return ZXing::BarcodeFormat::None;
    case EZXingFormat::Aztec:
        return ZXing::BarcodeFormat::Aztec;
    case EZXingFormat::Codabar:
        return ZXing::BarcodeFormat::Codabar;
    case EZXingFormat::Code39:
        return ZXing::BarcodeFormat::Code39;
    case EZXingFormat::Code93:
        return ZXing::BarcodeFormat::Code93;
    case EZXingFormat::Code128:
        return ZXing::BarcodeFormat::Code128;
    case EZXingFormat::DataBar:
        return ZXing::BarcodeFormat::DataBar;
    case EZXingFormat::DataBarExpanded:
        return ZXing::BarcodeFormat::DataBarExpanded;
    case EZXingFormat::DataMatrix:
        return ZXing::BarcodeFormat::DataMatrix;
    case EZXingFormat::EAN8:
        return ZXing::BarcodeFormat::EAN8;
    case EZXingFormat::EAN13:
        return ZXing::BarcodeFormat::EAN13;
    case EZXingFormat::ITF:
        return ZXing::BarcodeFormat::ITF;
    case EZXingFormat::MaxiCode:
        return ZXing::BarcodeFormat::MaxiCode;
    case EZXingFormat::PDF417:
        return ZXing::BarcodeFormat::PDF417;
    case EZXingFormat::QRCode:
        return ZXing::BarcodeFormat::QRCode;
    case EZXingFormat::UPCA:
        return ZXing::BarcodeFormat::UPCA;
    case EZXingFormat::UPCE:
        return ZXing::BarcodeFormat::UPCE;
    case EZXingFormat::MicroQRCode:
        return ZXing::BarcodeFormat::MicroQRCode;
    default:
        return ZXing::BarcodeFormat::None;
    }
}

void UFF_Nayuki_QRBPLibrary::ZXing_Encode(FDelegateTexture2D DelegateTexture2D, const FString& Text, EZXingFormat Format, FVector2D Resolution, int32 Margin, FColor BlackColor, FColor WhiteColor)
{
    if (Text.IsEmpty())
    {
        return;
    }

    if (Format == EZXingFormat::None)
    {
        return;
    }

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateTexture2D, Text, Format, Resolution, Margin, BlackColor, WhiteColor]()
        {
            ZXing::MultiFormatWriter Writer = ZXing::MultiFormatWriter(ConvertToBarcodeFormat(Format)).setMargin(Margin).setEncoding(ZXing::CharacterSet::UTF8);
            ZXing::BitMatrix Matrix = Writer.encode(TCHAR_TO_UTF8(*Text), Resolution.X, Resolution.Y);
            ZXing::Matrix<uint8_t> Qr_Matrix = ToMatrix<uint8_t>(Matrix);

            const size_t QR_BufferSize = Qr_Matrix.size();

            TArray<FColor> QRPixels;
            QRPixels.SetNumZeroed(QR_BufferSize);

            for (int32 Index_Pixel = 0; Index_Pixel < QR_BufferSize; Index_Pixel++)
            {
                FColor Color = Qr_Matrix.data()[Index_Pixel] ? WhiteColor : BlackColor;
                QRPixels[Index_Pixel] = Color;
            }

            AsyncTask(ENamedThreads::GameThread, [DelegateTexture2D, QR_BufferSize, Resolution, QRPixels]()
                {
                    UTexture2D* Texture = UTexture2D::CreateTransient(Resolution.X, Resolution.Y, PF_B8G8R8A8);
                    Texture->Filter = TextureFilter::TF_Nearest;
                    Texture->SRGB = false;
                    Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
                    Texture->CompressionNoAlpha = true;

#ifdef WITH_EDITOR
                    Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif // WITH_EDITOR

                    Texture->AddressX = TextureAddress::TA_Clamp;
                    Texture->AddressY = TextureAddress::TA_Clamp;

                    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
                    uint8* MipData = static_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));
                    FMemory::Memcpy(MipData, QRPixels.GetData(), QR_BufferSize * 4);

                    Mip.BulkData.Unlock();
                    Texture->UpdateResource();

                    DelegateTexture2D.ExecuteIfBound(true, "ZXing Encode is successful.", Texture);
                }
            );
        }
    );
}