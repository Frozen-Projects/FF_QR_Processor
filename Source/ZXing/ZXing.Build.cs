namespace UnrealBuildTool.Rules
{
    using System.IO;

    public class ZXing : ModuleRules
    {
        public ZXing(ReadOnlyTargetRules Target) : base(Target)
        {
    		Type = ModuleType.External;

            string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "core", "src"));
	    	PublicIncludePaths.Add(BaseDirectory);

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                string ZXingLibPath = Path.Combine(ModuleDirectory, "lib");
                PublicAdditionalLibraries.Add(Path.Combine(ZXingLibPath, "Win64", "ZXing.lib"));
            }
            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                string ZXingLibPath = Path.Combine(ModuleDirectory, "lib/Android/local");

                PublicAdditionalLibraries.AddRange(new string[] {
                    Path.Combine(ZXingLibPath, "arm64-v8a", "libzxing.a"),
                    Path.Combine(ZXingLibPath, "armeabi-v7a", "libzxing.a"),
                    Path.Combine(ZXingLibPath, "x86", "libzxing.a"),
                    Path.Combine(ZXingLibPath, "x86_64", "libzxing.a"),
                });
            }

            if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                string ZXingLibPath = Path.Combine(ModuleDirectory, "lib/Mac");
                ZXingLibPath = Path.Combine(ZXingLibPath, "libzxing.a");
                PublicAdditionalLibraries.Add(ZXingLibPath);
            }

            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                string ZXingLibPath = Path.Combine(ModuleDirectory, "lib/IOS");
                ZXingLibPath = Path.Combine(ZXingLibPath, "libzxing.a");
                PublicAdditionalLibraries.Add(ZXingLibPath);
            }
        }
    }
}
