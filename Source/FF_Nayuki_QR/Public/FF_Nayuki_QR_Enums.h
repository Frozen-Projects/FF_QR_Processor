#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EQrErrorTolerance : uint8
{
	Quartile	UMETA(DisplayName = "Quartile"),
	Low			UMETA(DisplayName = "Low"),
	Mid			UMETA(DisplayName = "Mid"),
	High		UMETA(DisplayName = "High"),
};
ENUM_CLASS_FLAGS(EQrErrorTolerance)