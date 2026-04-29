// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
//
#include "WarwolfMissionInterface.h"
#include "Components/StateTreeComponent.h"
//
#include "WarwolfMissionManager.generated.h"

/**
 * State tree component responsible for controlling mission logic. Should be placed on the Game State.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARWOLFMISSIONSYSTEM_API UWarwolfMissionManager : public UStateTreeComponent, public IWarwolfMissionInterface
{
	GENERATED_BODY()
	
	// IWarwolfMissionInterface
	virtual void SendMissionEvent_Implementation(const FStateTreeEvent& Event) override;
	// ~IWarwolfMissionInterface

public:

	/* @brief Set the state tree to use for this mission.
	 * @param InStateTree - State tree to run.
	 */
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void SetMissionStateTree(UStateTree* InStateTree);
};
