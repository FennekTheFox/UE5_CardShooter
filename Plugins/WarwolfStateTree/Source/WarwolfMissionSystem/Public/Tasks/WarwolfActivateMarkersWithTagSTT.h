// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfActivateMarkersWithTagSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfActivateMarkersWithTagsTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Gameplay tags to look for.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FGameplayTagContainer TagsToFind = FGameplayTagContainer();
	
};

/**
 * A task that looks for AWarwolfMissionMarker actors with tags specified in task data, and activates them.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Activate All Markers With Tags"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfActivateMarkersWithTagsTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfActivateMarkersWithTagsTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfActivateMarkersWithTagsTaskInstanceData::StaticStruct(); }
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