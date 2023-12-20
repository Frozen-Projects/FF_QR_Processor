// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes
#include "FF_QR_Include.h"

#include "FF_Nayuki_QRBPLibrary.generated.h"



USTRUCT(BlueprintType)
struct FF_NAYUKI_QR_API FZXingScanResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	EZXingFormat QR_Format;
	
	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	FString QR_Text;

	// Detected corner of the barcode, arranged in the order of top left, bottom left, bottom right and top right.
	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	TArray<FVector2D> QR_Points;
};

UCLASS()
class UFF_Nayuki_QRBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Nayuki QR - Encode QR Code", ToolTip = "Description.", Keywords = "generate, encode, qr, nayuki"), Category = "Frozen Forest|QR|Nayuki")
	static void NayukiQr_GenerateQRCode(FDelegateTexture2D DelegateTexture2D, const FString Text, FVector2D Resolution = FVector2D(512, 512), ENayukiQrTolerance ErrorTolerance = ENayukiQrTolerance::High, FColor BlackColor = FColor::Black, FColor WhiteColor = FColor::White);

	static EZXingFormat ZXing_ConvertToBpFormat(ZXing::BarcodeFormat Format);
	static ZXing::BarcodeFormat ZXing_ConvertToBarcodeFormat(EZXingFormat Format);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZXing - Encode QR Code", ToolTip = "Description.", Keywords = "generate, encode, qr, zxing"), Category = "Frozen Forest|QR|ZXing")
	static void ZXing_Encode(FDelegateTexture2D DelegateTexture2D, const FString Text, EZXingFormat Format, FVector2D Resolution, int32 Margin, FColor BlackColor = FColor::Black, FColor WhiteColor = FColor::White);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZXing - Decode QR Code", ToolTip = "Description.", Keywords = "decode, qr, zxing"), Category = "Frozen Forest|QR|ZXing")
	static bool ZXing_Decode(TArray<FZXingScanResult>& OutResults, FString& Out_Code, const FVector4& InRect, TArray<uint8> In_Buffer, FVector2D In_Size = FVector2D(512, 512), EPixelFormat PixelFormat = EPixelFormat::PF_B8G8R8A8);

};
