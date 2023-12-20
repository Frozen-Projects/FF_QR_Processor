// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_QR_ProcessorBPLibrary.h"
#include "FF_QR_Processor.h"

UFF_QR_ProcessorBPLibrary::UFF_QR_ProcessorBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UFF_QR_ProcessorBPLibrary::NayukiQr_GenerateQRCode(FDelegateTexture2D DelegateTexture2D, const FString In_Text, FVector2D Resolution, int32 Border, ENayukiQrTolerance ErrorTolerance, FColor BlackColor, FColor WhiteColor)
{
    if (In_Text.IsEmpty())
    {
        return;
    }

    if (Resolution.X == 0 || Resolution.Y == 0)
    {
        return;
    }

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateTexture2D, In_Text, Resolution, Border, ErrorTolerance, BlackColor, WhiteColor]()
        {
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
                ToleranceLevel = qrcodegen::QrCode::Ecc::LOW;
                break;
            }

            qrcodegen::QrCode QRCode = qrcodegen::QrCode::encodeText(TCHAR_TO_UTF8(*In_Text), ToleranceLevel);

            int32 QRSize = QRCode.getSize();

            if (Resolution.X < QRSize || Resolution.Y < QRSize)
            {
                AsyncTask(ENamedThreads::GameThread, [DelegateTexture2D, QRSize]()
                    {
                        DelegateTexture2D.ExecuteIfBound(false, ("Nayuki QR encode is NOT successful. Resolution shouldn't smaller than actual QR size :" + FString::FromInt(QRSize) ), nullptr);
                        return;
                    }
                );
            }

            int32 Margin = Border * 2;

            TArray<FColor> QR_Pixels_Raw;
            QR_Pixels_Raw.Init(BlackColor, (QRSize + Margin) * (QRSize + Margin));
            
            for (int32 x = 0; x < QRSize + 0; x++)
            {
                for (int32 y = 0; y < QRSize + 0; y++)
                {
                    int32 Index_Pixel = ((x + Border) + (y + Border) * (QRSize + Margin));
                    FColor EachPixel = QRCode.getModule(x, y) ? WhiteColor : BlackColor;
                    QR_Pixels_Raw[Index_Pixel] = EachPixel;
                }
            }

            AsyncTask(ENamedThreads::GameThread, [DelegateTexture2D, Resolution, QRSize, QR_Pixels_Raw, Margin]()
                {
                    TArray<FColor> QR_Pixels_Resized;
                    QR_Pixels_Resized.SetNumZeroed(Resolution.X * Resolution.Y);
                    FImageUtils::ImageResize(QRSize + Margin, QRSize + Margin, QR_Pixels_Raw, Resolution.X, Resolution.Y, QR_Pixels_Resized, false);

                    UTexture2D* Texture = UTexture2D::CreateTransient(Resolution.X, Resolution.Y, EPixelFormat::PF_B8G8R8A8, "QRCode");
                    Texture->Filter = TextureFilter::TF_Nearest;
                    Texture->SRGB = false;
                    Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
                    Texture->CompressionNoAlpha = true;

                    void* data = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_ONLY);
                    FMemory::Memcpy(data, QR_Pixels_Resized.GetData(), QR_Pixels_Resized.Num() * 4);
                    Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

                    Texture->UpdateResource();

                    if (IsValid(Texture))
                    {
                        DelegateTexture2D.ExecuteIfBound(true, "Nayuki QR encode is successful.", Texture);
                        return;
                    }

                    else
                    {
                        DelegateTexture2D.ExecuteIfBound(false, "Nayuki QR encode is NOT successful.", nullptr);
                        return;
                    }
                }
            );

            return;
        }
    );
}

EZXingFormat UFF_QR_ProcessorBPLibrary::ZXing_ConvertToBpFormat(ZXing::BarcodeFormat Format)
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

ZXing::BarcodeFormat UFF_QR_ProcessorBPLibrary::ZXing_ConvertToBarcodeFormat(EZXingFormat Format)
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

void UFF_QR_ProcessorBPLibrary::ZXing_Encode(FDelegateTexture2D DelegateTexture2D, const FString In_Text, EZXingFormat Format, FVector2D Resolution, int32 Border, FColor BlackColor, FColor WhiteColor)
{
    if (In_Text.IsEmpty())
    {
        return;
    }

    if (Format == EZXingFormat::None)
    {
        return;
    }

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateTexture2D, In_Text, Format, Resolution, Border, BlackColor, WhiteColor]()
        {
            ZXing::MultiFormatWriter Writer = ZXing::MultiFormatWriter(UFF_QR_ProcessorBPLibrary::ZXing_ConvertToBarcodeFormat(Format)).setMargin(Border).setEncoding(ZXing::CharacterSet::UTF8);
            ZXing::BitMatrix Matrix = Writer.encode(TCHAR_TO_UTF8(*In_Text), Resolution.X, Resolution.Y);
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

                    if (IsValid(Texture))
                    {
                        DelegateTexture2D.ExecuteIfBound(true, "ZXing Encode is successful.", Texture);
                        return;
                    }

                    else
                    {
                        DelegateTexture2D.ExecuteIfBound(false, "ZXing Encode is NOT successful.", nullptr);
                        return;
                    }   
                }
            );

            return;
        }
    );
}

bool UFF_QR_ProcessorBPLibrary::ZXing_Decode(TArray<FZXingScanResult>& OutResults, FString& Out_Code, const FVector4& InRect, TArray<uint8> In_Buffer, FVector2D In_Size, EPixelFormat PixelFormat)
{
    if (In_Buffer.IsEmpty())
    {
        Out_Code = "Target buffer is empty.";
        return false;
    }

    ZXing::ImageFormat ZXing_Image_Format = ZXing::ImageFormat::None;

    switch (PixelFormat)
    {
    case PF_Unknown:
        
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_A32B32G32R32F:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_B8G8R8A8:
        
        ZXing_Image_Format = ZXing::ImageFormat::BGRX;
        break;

    case PF_G8:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_G16:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_DXT1:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_DXT3:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_DXT5:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_UYVY:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_FloatRGB:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_FloatRGBA:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_DepthStencil:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ShadowDepth:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R32_FLOAT:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_G16R16:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_G16R16F:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_G16R16F_FILTER:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_G32R32F:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_A2B10G10R10:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_A16B16G16R16:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_D24:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R16F:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R16F_FILTER:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_BC5:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_V8U8:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_A1:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_FloatR11G11B10:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_A8:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R32_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R32_SINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_PVRTC2:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_PVRTC4:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R16_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R16_SINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R16G16B16A16_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R16G16B16A16_SINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    case PF_R5G6B5_UNORM:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R8G8B8A8:
        
        ZXing_Image_Format = ZXing::ImageFormat::RGBX;
        break;

    case PF_A8R8G8B8:
        
        ZXing_Image_Format = ZXing::ImageFormat::XRGB;
        break;

    case PF_BC4:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R8G8:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ATC_RGB:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ATC_RGBA_E:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ATC_RGBA_I:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_X24_G8:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ETC1:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ETC2_RGB:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ETC2_RGBA:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R32G32B32A32_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R16G16_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ASTC_4x4:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ASTC_6x6:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ASTC_8x8:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_10x10:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ASTC_12x12:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_BC6H:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_BC7:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R8_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_L8:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_XGXR8:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R8G8B8A8_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R8G8B8A8_SNORM:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R16G16B16A16_UNORM:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R16G16B16A16_SNORM:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_PLATFORM_HDR_0:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_PLATFORM_HDR_1:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_PLATFORM_HDR_2:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_NV12:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R32G32_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ETC2_R11_EAC:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_ETC2_RG11_EAC:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_R8:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_B5G5R5A1_UNORM:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_4x4_HDR:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_6x6_HDR:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_8x8_HDR:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_10x10_HDR:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_12x12_HDR:
        Out_Code = "Unsupported pixel format.";
        return false;
   
    case PF_G16R16_SNORM:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R8G8_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R32G32B32_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R32G32B32_SINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R32G32B32F:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R8_SINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R64_UINT:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_R9G9B9EXP5:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_P010:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_4x4_NORM_RG:
        Out_Code = "Unsupported pixel format.";
        return false;
        
    case PF_ASTC_6x6_NORM_RG:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_8x8_NORM_RG:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_10x10_NORM_RG:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_ASTC_12x12_NORM_RG:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    case PF_MAX:
        Out_Code = "Unsupported pixel format.";
        return false;
    
    default:
        
        ZXing_Image_Format = ZXing::ImageFormat::BGRX;
        break;
    }

    FIntRect Rect(InRect.X, InRect.Y, InRect.Z, InRect.W);
    if (Rect == FIntRect(0, 0, 0, 0))
    {
        Rect = FIntRect(0, 0, In_Size.X, In_Size.Y);
    }

    ZXing::ImageView ZXing_Image
    { 
        reinterpret_cast<uint8*>(In_Buffer.GetData()), Rect.Width(), Rect.Height(), ZXing_Image_Format
    };

    ZXing::DecodeHints hints;
    hints.setTextMode(ZXing::TextMode::HRI);
    hints.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);
    ZXing::Results Results = ZXing::ReadBarcodes(ZXing_Image, hints);
    if (!Results.empty())
    {
        for (int32 i = 0; i < Results.size(); i++)
        {
            FZXingScanResult Result;

            Result.QR_Text = ANSI_TO_TCHAR(Results[i].text().c_str());

            FVector2D TopLeft = FVector2D(Results[i].position().topLeft().x, Results[i].position().topLeft().y);
            FVector2D TopRight = FVector2D(Results[i].position().topRight().x, Results[i].position().topRight().y);
            FVector2D BottomLeft = FVector2D(Results[i].position().bottomLeft().x, Results[i].position().bottomLeft().y);
            FVector2D BottomRight = FVector2D(Results[i].position().bottomRight().x, Results[i].position().bottomRight().y);

            Result.QR_Points.Add(FVector2D(TopLeft));
            Result.QR_Points.Add(FVector2D(BottomLeft));
            Result.QR_Points.Add(FVector2D(BottomRight));
            Result.QR_Points.Add(FVector2D(TopRight));

            Result.QR_Format = UFF_QR_ProcessorBPLibrary::ZXing_ConvertToBpFormat(Results[i].format());

            OutResults.Add(Result);
        }

        Out_Code = "QR decode with ZXing is successful.";
        return true;
    }

    else
    {
        Out_Code = "QR decode with ZXing is NOT successful.";
        return false;
    }

}