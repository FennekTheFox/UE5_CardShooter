

using UnrealBuildTool;

public class CardShooter : ModuleRules
{
    public CardShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "CommonUI",
            "CommonInput",
            "EnhancedInput",
            "AIModule",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "UMG",
            "Slate",
            "SlateCore",
            "GameplayTags",
            "GameplayAbilities",
            "GameplayTasks",
            "ProjectAbilitySystem_Plugin",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemEOS",
            "CoreOnline",
            "ModularGameplay",
            "ModularGameplayActors",
            "GameFeatures",
            "LyraGame"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "OnlineServicesInterface",
            "OnlineServicesCommon",
            "OnlineServicesEOS",
            "OnlineServicesEOSGS",
            "EOSShared"
        });

        PublicIncludePaths.AddRange(new string[] {
            "CardShooter",
            "CardShooter/Variant_Horror",
            "CardShooter/Variant_Horror/UI",
            "CardShooter/Variant_Shooter",
            "CardShooter/Variant_Shooter/AI",
            "CardShooter/Variant_Shooter/UI",
            "CardShooter/Variant_Shooter/Weapons"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
