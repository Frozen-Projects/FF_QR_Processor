namespace UnrealBuildTool.Rules
{
    using System.IO;

    public class ZXing : ModuleRules
    {
        public ZXing(ReadOnlyTargetRules Target) : base(Target)
        {
    		Type = ModuleType.External;
            UndefinedIdentifierWarningLevel = WarningLevel.Off;
            bEnableExceptions = true;
            bUseRTTI = true;

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Win64", "include", "src"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "ZXing.lib"));
            }

            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Android", "include", "src"));
                
                string AndroidLibs = Path.Combine(ModuleDirectory, "Android", "lib");

                PublicAdditionalLibraries.AddRange(new string[] {
                    Path.Combine(AndroidLibs, "arm64-v8a", "libzxing.a"),
                    Path.Combine(AndroidLibs, "armeabi-v7a", "libzxing.a"),
                    Path.Combine(AndroidLibs, "x86", "libzxing.a"),
                    Path.Combine(AndroidLibs, "x86_64", "libzxing.a"),
                });

                /*
                PublicAdditionalLibraries.AddRange(new string[] 
                {
                    Path.Combine(AndroidLibs, "arm64-v8a", "libzxingcpp_android.so"),
                    Path.Combine(AndroidLibs, "armeabi-v7a", "libzxingcpp_android.so"),
                    Path.Combine(AndroidLibs, "x86", "libzxingcpp_android.so"),
                    Path.Combine(AndroidLibs, "x86_64", "libzxingcpp_android.so"),
                });

                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "ZXing_UPL_Android.xml"));
                */
            }
        }
    }
}
