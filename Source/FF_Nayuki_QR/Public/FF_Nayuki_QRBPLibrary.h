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
	EZXingFormat Format;
	
	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	FText Text;

	// Detected corner of the barcode, arranged in the order of top left, bottom left, bottom right and top right.
	UPROPERTY(BlueprintReadWrite, Category = "ZXing")
	TArray<FVector2D> Points;
};

UCLASS()
class UFF_Nayuki_QRBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Nayuki QR - Generate QR Code", ToolTip = "Description.", Keywords = "generate, qr"), Category = "Frozen Forest|QR|Nayuki")
	static bool NayukiQr_GenerateQRCode(UTexture2D*& OutQRCode, const FString QR_String, ENayukiQrTolerance ErrorTolerance);

	static EZXingFormat ConvertToBlueprintFormat(ZXing::BarcodeFormat Format);
	static ZXing::BarcodeFormat ConvertToBarcodeFormat(EZXingFormat Format);

	UFUNCTION(BlueprintCallable, Category = "Frozen Forest|QR|ZXing")
	static void ZXing_Encode(FDelegateTexture2D DelegateTexture2D, const FString& Text, EZXingFormat Format, FVector2D Resolution, int32 Margin, FColor BlackColor = FColor::Black, FColor WhiteColor = FColor::White);

};
