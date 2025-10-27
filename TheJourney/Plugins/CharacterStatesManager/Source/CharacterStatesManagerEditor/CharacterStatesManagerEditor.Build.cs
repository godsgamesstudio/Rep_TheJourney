using UnrealBuildTool;
using System.IO;

public class CharacterStatesManagerEditor : ModuleRules
{
    public CharacterStatesManagerEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core" 
        });

        // Módulos que o nosso módulo de editor precisa
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "BlueprintGraph",       
            "KismetCompiler",
            "CharacterStatesManager", 
            "CoreUObject", 
            "Engine",
            "UnrealEd",
            "AssetTools"
        });
    }
}
