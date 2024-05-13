// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_QR_Processor.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FFF_QR_ProcessorModule"

void FFF_QR_ProcessorModule::StartupModule()
{
#ifdef _WIN64
	const FString BasePluginDir = IPluginManager::Get().FindPlugin("FF_QR_PROCESSOR")->GetBaseDir();
	const FString Path_OpenCV = FPaths::Combine(*BasePluginDir, TEXT("Source/OpenCV_Libs/Win64/lib/opencv_world490.dll"));
	const FString Path_OpenCV_D = FPaths::Combine(*BasePluginDir, TEXT("Source/OpenCV_Libs/Win64/lib/opencv_world490d.dll"));

	this->Handle_OpenCV = FPlatformProcess::GetDllHandle(*Path_OpenCV);
	this->Handle_OpenCV_D = FPlatformProcess::GetDllHandle(*Path_OpenCV_D);

	if (Handle_OpenCV != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("opencv_world490.dll loaded successfully!"));
	}

	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("opencv_world490.dll failed to load!"));
	}

	if (Handle_OpenCV_D != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("opencv_world490d.dll loaded successfully!"));
	}

	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("opencv_world490d.dll failed to load!"));
	}
#endif
}

void FFF_QR_ProcessorModule::ShutdownModule()
{
#ifdef _WIN64
	FPlatformProcess::FreeDllHandle(this->Handle_OpenCV);
	this->Handle_OpenCV = nullptr;

	FPlatformProcess::FreeDllHandle(this->Handle_OpenCV_D);
	this->Handle_OpenCV_D = nullptr;
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFF_QR_ProcessorModule, FF_QR_Processor)