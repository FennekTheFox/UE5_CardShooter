// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
//
#include "GameplayTagContainer.h"
//
#include "WarwolfMissionTagInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWarwolfMissionTagInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is for interacting between mission-related logic and the mission system.
 */
class WARWOLFMISSIONSYSTEM_API IWarwolfMissionTagInterface
{
	GENERATED_BODY()

public:

	/*
	 * @brief Get the mission-related gameplay tags from this actor.
	 * @return Gameplay tag containers for this owner actor.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	FGameplayTagContainer GetMissionTags();
	
};
