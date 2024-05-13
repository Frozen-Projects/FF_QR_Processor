// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes
#include "FF_QR_Includes.h"

#include "FF_QR_ProcessorBPLibrary.generated.h"

USTRUCT(BlueprintType)
struct FF_QR_PROCESSOR_API FZXingScanResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	EZXingFormat QR_Format;
	
	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	FString QR_Text;

	// Detected corner of the barcode, arranged in the order of top left, bottom left, bottom right and top right.
	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	TArray<FVector2D> QR_Points;
};

UCLASS()
class UFF_QR_ProcessorBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Nayuki QR - Encode QR Code", ToolTip = "Description.", Keywords = "generate, encode, qr, nayuki"), Category = "Frozen Forest|QR|Nayuki")
	static FF_QR_PROCESSOR_API void NayukiQr_GenerateQRCode(FDelegateTexture2D DelegateTexture2D, const FString Text, FVector2D Resolution = FVector2D(512, 512), int32 Border = 0, ENayukiQrTolerance ErrorTolerance = ENayukiQrTolerance::High, FColor BlackColor = FColor::Black, FColor WhiteColor = FColor::White);

	static FF_QR_PROCESSOR_API EZXingFormat ZXing_ConvertToBpFormat(ZXing::BarcodeFormat Format);
	static FF_QR_PROCESSOR_API ZXing::BarcodeFormat ZXing_ConvertToBarcodeFormat(EZXingFormat Format);
	static FF_QR_PROCESSOR_API bool ZXing_Decoder_Callback(TArray<FZXingScanResult>& OutResults, FString& Out_Code, uint8* In_Buffer, FIntRect Rect, ZXing::ImageFormat ZXing_Image_Format);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZXing - Encode QR Code", ToolTip = "Description.", Keywords = "generate, encode, qr, zxing"), Category = "Frozen Forest|QR|ZXing")
	static FF_QR_PROCESSOR_API void ZXing_Encode(FDelegateTexture2D DelegateTexture2D, const FString Text, EZXingFormat Format = EZXingFormat::QRCode, FVector2D Resolution = FVector2D(512, 512), int32 Border = 0, FColor BlackColor = FColor::Black, FColor WhiteColor = FColor::White);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZXing - Decode QR Code", ToolTip = "Description.", Keywords = "decode, qr, zxing"), Category = "Frozen Forest|QR|ZXing")
	static FF_QR_PROCESSOR_API bool ZXing_Decode(TArray<FZXingScanResult>& Out_Results, FString& Out_Code, const FVector4& In_Rect, TArray<uint8> In_Buffer, FVector2D In_Size = FVector2D(512, 512), EPixelFormat PixelFormat = EPixelFormat::PF_B8G8R8A8);

	static FF_QR_PROCESSOR_API bool OpenCV_QR_Decoder_Callback(FString& DecodedString, uint8* Buffer, FVector2D ImageSize);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZXing - Decode QR Code", ToolTip = "Description.", Keywords = "decode, qr, zxing"), Category = "Frozen Forest|QR|ZXing")
	static FF_QR_PROCESSOR_API bool OpenCV_QR_Decoder(FString& DecodedString, TArray<uint8> Buffer, FVector2D ImageSize);
};