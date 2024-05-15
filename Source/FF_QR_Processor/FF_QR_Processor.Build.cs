// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;

public class FF_QR_Processor : ModuleRules
{
	public FF_QR_Processor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;

        bUseUnity = false;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "nayuki_qr"));

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"ZXing",
				"OpenCV_Libs",
            }
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Projects",
            }
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}