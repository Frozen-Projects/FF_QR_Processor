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
        bUseRTTI = true;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "nayuki_qr"));

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"ZXing",
                "ExtendedVars",		// Other plugins which use this plugin, requires ExtendedVars. This satisy that requirement.
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
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