// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionActivateObjSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfActivateObjTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Tags of missions to start. Do not include .Start leaf tag. Only the parent mission tag.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FGameplayTagContainer MissionTags = FGameplayTagContainer();
	
};

/**
 * A task that sends objective start events for the given objective tags
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Send Objective Start Event"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfActivateObjTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfActivateObjTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfActivateObjTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Event");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR
	
};