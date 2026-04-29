// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarwolfMissionPlaySequenceSTT.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfMissionTaskHelper.generated.h"

UCLASS()
/*
 * A utility class to manage dynamic delegate bindings for level sequence playback.
 * This helper is necessary because dynamic delegates (e.g., OnFinished) in Unreal Engine
 * require a UObject-based target and do not directly support binding to const functions.
 *
 * This class serves as an intermediary, forwarding the delegate call to the associated
 * task instance, ensuring compatibility with const methods in StateTree tasks.
 */
class UWarwolfMissionTaskHelper : public UObject
{
	GENERATED_BODY()

public:
	// @brief Handles the delegate call when the level sequence finishes playing.
	UFUNCTION()
	void OnLevelSequenceFinished()
	{
		if (!TaskInstance)
		{
			UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s TaskInstance is not valid. Returning fail state."), *FString(__FUNCTION__));
			return;
		}

		const FWarwolfPlaySequenceTask* Task = static_cast<const FWarwolfPlaySequenceTask*>(TaskInstance);
		if (!Task)
		{
			UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Task is not valid. Returning fail state."), *FString(__FUNCTION__));
			return;
		}
		
		Task->LevelSequenceFinished();
		MarkAsGarbage(); // Clean up since this helper has finished its job.
	}

	// @brief Pointer to the task instance that owns this delegate binding. 
	const void* TaskInstance = nullptr;
};