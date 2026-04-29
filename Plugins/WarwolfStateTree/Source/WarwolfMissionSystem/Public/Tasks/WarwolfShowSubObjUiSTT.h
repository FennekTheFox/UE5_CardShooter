// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfShowSubObjUiSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfShowSubObjectiveUiTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Text we want to display on the sub-objective widget.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FText SubObjectiveText = FText();

	// @brief Does this sub-objective need a parameter on the state tree? (e.g. so a counting task can access it)
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	bool bHasParamOnStateTree = false;

	// @brief Reference to this Sub-Objective Widget Guid parameter in the state tree
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid", EditCondition="bHasParamOnStateTree", Optional), Category="WarwolfMissionSystem")
	FStateTreePropertyRef SubObjectiveWidgetGuid;

	UPROPERTY(Transient, meta=(HideInDetailsView))
	FGuid WidgetGuid = FGuid();

	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid"), Category="WarwolfMissionSystem")
	FStateTreePropertyRef ObjectiveWidgetGuid;

};

/**
 * A task that shows a sub-objective when entering the state, and hides it once the state has finished.
 * Automatically removes itself from the HUD when the state has finished. If you want to keep this up, use nested states.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Show Sub-Objective On HUD"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfShowSubObjectiveUiTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfShowSubObjectiveUiTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfShowSubObjectiveUiTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

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