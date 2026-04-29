// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
//
#include "StateTreeEvents.h"
//
#include "WarwolfMissionInterface.generated.h"

class UWarwolfMissionDataAsset;

UINTERFACE(MinimalAPI)
class UWarwolfMissionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is for interacting between mission-related logic and the mission system.
 */
class WARWOLFMISSIONSYSTEM_API IWarwolfMissionInterface
{
	GENERATED_BODY()

public:

	/*
	 * @brief Select a mission based on the mission data given.
	 * @param InMissionData - Data asset of mission we want to load.
	 * @param bStartMissionInstantly - Should we begin the mission right away or wait until it's started manually?
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void SelectMission(UWarwolfMissionDataAsset* InMissionData, const bool bStartMissionInstantly);

	// @brief Start the mission logic if it hasn't already been started.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void StartMission();

	/*
	 * @brief Send an event to the mission state tree.
	 * @param Event - State tree event to send to the mission system.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void SendMissionEvent(const FStateTreeEvent& Event);
	
};
