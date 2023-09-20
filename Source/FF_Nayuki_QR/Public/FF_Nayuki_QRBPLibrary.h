// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes
#include "FF_Nayuki_QR_Enums.h"

#include "FF_Nayuki_QRBPLibrary.generated.h"

UCLASS()
class UFF_Nayuki_QRBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate QR Code", ToolTip = "Description.", Keywords = "generate, qr"), Category = "Device Infos|IDs")
	static bool GenerateQRCode(UTexture2D*& OutQRCode, const FString QR_String, EQrErrorTolerance ErrorTolerance);

};
