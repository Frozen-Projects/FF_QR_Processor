// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FFF_QR_ProcessorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#ifdef _WIN64

	void* Handle_OpenCV = nullptr;
	//void* Handle_Intel_Realsense;

	void* Handle_OpenCV_D = nullptr;
	//void* Handle_Intel_Realsense;

#endif
};