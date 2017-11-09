// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class ZombiETS : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../")); }
    }

    public ZombiETS(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));

        LoadLibrary(Target, "avutil");
        LoadLibrary(Target, "avformat");
        LoadLibrary(Target, "avcodec");
        LoadLibrary(Target, "avfilter");
        LoadLibrary(Target, "BeatFinder");
    }

    public bool LoadLibrary(TargetInfo Target, string LibraryName)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "lib");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, LibraryName + ".lib"));
        }

        Definitions.Add(string.Format("WITH_" + LibraryName.ToUpper() + "_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
