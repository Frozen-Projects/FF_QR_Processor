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

bool UFF_QR_ProcessorBPLibrary::ZXing_Decoder_Callback(TArray<FZXingScanResult>& OutResults, FString& Out_Code, uint8* In_Buffer, FIntRect Rect, ZXing::ImageFormat ZXing_Image_Format)
{
    if (!In_Buffer)
    {
        Out_Code = "QR decode with ZXing is NOT successful. Buffer is not valid.";
        return false;
    }

    if (Rect.Height() == 0 && Rect.Width() == 0)
    {
        Out_Code = "QR decode with ZXing is NOT successful. Width and height shouldn't be 0.";
        return false;
    }

    ZXing::ImageView ZXing_Image
    {
        In_Buffer, Rect.Width(), Rect.Height(), ZXing_Image_Format
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
    
    return false;
}

bool UFF_QR_ProcessorBPLibrary::ZXing_Decode(TArray<FZXingScanResult>& Out_Results, FString& Out_Code, const FVector4& In_Rect, TArray<uint8> In_Buffer, FVector2D In_Size, EPixelFormat PixelFormat)
{
    if (In_Buffer.IsEmpty())
    {
        Out_Code = "Target buffer is empty.";
        return false;
    }

    ZXing::ImageFormat ZXing_Image_Format = ZXing::ImageFormat::None;

    switch (PixelFormat)
    {
        case PF_B8G8R8A8:
        
            ZXing_Image_Format = ZXing::ImageFormat::BGRX;
            break;
    
        case PF_R8G8B8A8:
        
            ZXing_Image_Format = ZXing::ImageFormat::RGBX;
            break;

        case PF_A8R8G8B8:
        
            ZXing_Image_Format = ZXing::ImageFormat::XRGB;
            break;
    
        default:
        
            Out_Code = "Unsupported texture format";
            return false;
    }

    FIntRect Rect(In_Rect.X, In_Rect.Y, In_Rect.Z, In_Rect.W);
    if (Rect == FIntRect(0, 0, 0, 0))
    {
        Rect = FIntRect(0, 0, In_Size.X, In_Size.Y);
    }

    return UFF_QR_ProcessorBPLibrary::ZXing_Decoder_Callback(Out_Results, Out_Code, In_Buffer.GetData(), Rect, ZXing_Image_Format);
}

bool UFF_QR_ProcessorBPLibrary::OpenCV_QR_Decoder_Callback(FString& DecodedString, uint8* Buffer, FVector2D ImageSize)
{
    if (!Buffer)
    {
        return false;
    }

    if (ImageSize.X == 0 || ImageSize.Y == 0)
    {
        return false;
    }

    cv::Mat Image(cv::Size((int32)ImageSize.X, (int32)ImageSize.Y), CV_8UC4, Buffer);
    cv::Mat Points;
    cv::Mat Output;

    cv::QRCodeDetector QR_Detector;

#ifdef _WIN64
    QR_Detector = cv::QRCodeDetector::QRCodeDetector();
#endif

#ifdef __ANDROID__
    QR_Detector = cv::QRCodeDetector();
#endif

    DecodedString = QR_Detector.detectAndDecode(Image, Points, Output).c_str();

    return true;
}

bool UFF_QR_ProcessorBPLibrary::OpenCV_QR_Decoder(FString& DecodedString, TArray<uint8> Buffer, FVector2D ImageSize)
{
    if (Buffer.IsEmpty())
    {
        return false;
    }

    return UFF_QR_ProcessorBPLibrary::OpenCV_QR_Decoder_Callback(DecodedString, Buffer.GetData(), ImageSize);
}