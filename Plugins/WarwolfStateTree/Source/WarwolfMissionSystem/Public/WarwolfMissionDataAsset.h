// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateTree.h"
#include "WarwolfMissionTypes.h"
#include "Engine/DataAsset.h"
#include "WarwolfMissionDataAsset.generated.h"

/**
 * Data asset for holding all mission information.
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfMissionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	// @brief Name of the mission.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem")
	FText MissionName = FText::FromString("Default Mission Name");

	// @brief Description of the mission. Used for designer context.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem")
	FText MissionDescription = FText::FromString("Default Mission Description.");

	// @brief Gameplay tag associated with the mission.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem")
	FGameplayTag MissionTag = FGameplayTag();

	// @brief State tree used for mission.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem")
	TObjectPtr<UStateTree> MissionStateTree = nullptr;

	// @brief The type of mission this is.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem")
	EWarwolfMissionType MissionType = EWarwolfMissionType::Primary;
};
