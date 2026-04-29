// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfShowObjectiveUiSTT.generated.h"

class UWarwolfMissionDataAsset;

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfShowObjectiveUiTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Text we want to display on the objective widget.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FText ObjectiveText = FText();

	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid"), Category="WarwolfMissionSystem")
	FStateTreePropertyRef ObjectiveWidgetGuid;

	// @brief The mission data from the state tree parameters.
	UPROPERTY(EditAnywhere, Category = "Input", meta=(Required))
	TObjectPtr<UWarwolfMissionDataAsset> MissionData = nullptr;

};

/**
 * A task that shows an objective when entering the state.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Show Objective On HUD"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfShowObjectiveUiTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfShowObjectiveUiTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfShowObjectiveUiTaskInstanceData::StaticStruct(); }
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