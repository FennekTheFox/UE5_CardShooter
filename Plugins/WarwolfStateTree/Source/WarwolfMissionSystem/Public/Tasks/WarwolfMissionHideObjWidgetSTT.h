// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionHideObjWidgetSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfHideObjectiveUiTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid"), Category="WarwolfMissionSystem")
	FStateTreePropertyRef ObjectiveWidgetGuid;

};

/**
 * A task that hides an objective widget when entering the state.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Hide Objective On HUD"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfHideObjectiveUiTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfHideObjectiveUiTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfHideObjectiveUiTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Text");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR
	
};