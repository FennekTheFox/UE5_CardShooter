// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarwolfMissionSystem.h"

#include "GameplayTagsManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

// Fix redefined namespace error on android.
#ifdef LOCTEXT_NAMESPACE
#undef LOCTEXT_NAMESPACE
#endif // LOCTEXT_NAMESPACE
#define LOCTEXT_NAMESPACE "FWarwolfMissionSystemModule"

void FWarwolfMissionSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	for (const TSharedPtr<const IPlugin> ThisPlugin : IPluginManager::Get().GetEnabledPluginsWithContent())
	{
		if (FString TagsIniDirectory = ThisPlugin->GetBaseDir() / TEXT("Config") / TEXT("Tags");
		FPaths::DirectoryExists(TagsIniDirectory))
		{
			UE_LOG(LogWarwolfMissionSystem, Log, TEXT("TagIniSearchPath: %s Added"), *TagsIniDirectory);

			UGameplayTagsManager::Get().AddTagIniSearchPath(TagsIniDirectory);
		}
	}
}

void FWarwolfMissionSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWarwolfMissionSystemModule, WarwolfMissionSystem)