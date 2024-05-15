namespace UnrealBuildTool.Rules
{
    using System.IO;

    public class OpenCV_Libs : ModuleRules
    {
        public OpenCV_Libs(ReadOnlyTargetRules Target) : base(Target)
        {
            Type = ModuleType.External;
            bEnableUndefinedIdentifierWarnings = false;
            bEnableExceptions = true;
            bUseRTTI = true;

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Win64", "include"));

                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "opencv_world490.lib"));
                RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "opencv_world490.dll"));
                PublicDelayLoadDLLs.Add("opencv_world490.dll");

                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "opencv_world490d.lib"));
                RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "opencv_world490d.dll"));
                PublicDelayLoadDLLs.Add("opencv_world490d.dll");
            }

            /*
            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Android", "include"));
                
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_calib3d.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_core.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_dnn.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_features2d.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_flann.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_gapi.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_highgui.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_imgcodecs.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_imgproc.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_ml.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_objdetect.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_photo.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_stitching.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_video.a"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "lib", "libopencv_videoio.a"));
            }
            */
        }
    }
}