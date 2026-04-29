// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfDeactivateMarkersSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfDeactivateMarkersWithTagsTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Gameplay tags to look for.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FGameplayTagContainer TagsToFind = FGameplayTagContainer();
	
};

/**
 * A task that looks for AWarwolfMissionMarker actors with tags specified in task data, and deactivates them.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Deactivate All Markers With Tags"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfDeactivateMarkersWithTagsTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfDeactivateMarkersWithTagsTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfDeactivateMarkersWithTagsTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Find");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR

	// @brief World from task context.
	UPROPERTY()
	mutable TObjectPtr<UWorld> World = nullptr;
	
};