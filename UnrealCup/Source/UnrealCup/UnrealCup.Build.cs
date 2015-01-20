// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class UnrealCup : ModuleRules
{

	private string ModulePath
	{
		get { return Path.GetDirectoryName( RulesCompiler.GetModuleFilename( this.GetType().Name ) ); }
	}

	private string ThirdPartyPath
	{
		get { return Path.GetFullPath( Path.Combine( ModulePath, "../../ThirdParty/" ) ); }
	}
		

	public UnrealCup(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		LoadLua(Target);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
	
	
	public bool LoadLua(TargetInfo Target)
    {
        bool isLibrarySupported = false;
 
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
 
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "lua", "Libraries");
 
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "Lua." + PlatformString + ".lib")); 
        }
 
        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add( Path.Combine( ThirdPartyPath, "lua", "Include" ) );
        }
 
        Definitions.Add(string.Format( "WITH_LUA_BINDING={0}", isLibrarySupported ? 1 : 0 ) );
 
        return isLibrarySupported;
    }
	
}
