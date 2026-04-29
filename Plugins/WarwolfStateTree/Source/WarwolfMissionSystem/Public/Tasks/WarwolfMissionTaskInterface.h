// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequencePlayer.h"
#include "UObject/Interface.h"
#include "WarwolfMissionTaskInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWarwolfIntCount);

UINTERFACE(MinimalAPI)
class UWarwolfMissionTaskInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is for interacting between mission state tree tasks and the mission system.
 */
class WARWOLFMISSIONSYSTEM_API IWarwolfMissionTaskInterface
{
	GENERATED_BODY()

public:

	// @brief Activate this actor. (Call from task, implement on an actor.)
	UFUNCTION(BlueprintNativeEvent, Category = "WarwolfMissionSystem")
	void ActivateMissionActor();

	// @brief Deactivate this actor. (Call from task, implement on an actor.)
	UFUNCTION(BlueprintNativeEvent, Category = "WarwolfMissionSystem")
	void DeactivateMissionActor();

	/**
	 * @brief Plays the provided Level Sequence.
	 * @param InLevelSequence - The sequence to play.
	 * @return the Level Sequence Player, which can be used to bind to events like OnFinished.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "WarwolfMissionSystem")
	ULevelSequencePlayer* PlayLevelSequence(ULevelSequence* InLevelSequence);

	/*
	 * @brief Get a delegate reference for classes that expect to increment an int in a state tree task.
	 * @return a reference to a delegate we broadcast whenever we expect an int to increment.
	 */
	virtual FOnWarwolfIntCount& GetIntCountDelegate() = 0;
	
};
