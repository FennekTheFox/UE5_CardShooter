// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfMissionTimerSTT.generated.h"

class UWarwolfMissionTimerTask;
USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfTimerTaskInstanceData
{
	GENERATED_BODY()

	// @brief How many seconds do we count to?
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	float FinalTime = 0.f;

	// @brief Do we want to count down from the final time instead of up to it? (Only affects UI)
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	bool bCountDown = false;

	// @brief Do we want to update a sub-objective when something is counted?
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	bool bUpdateSubObjective = false;

	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree that we want to update with time.
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid", EditCondition="bUpdateSubObjective", Optional), Category="WarwolfMissionSystem")
	FStateTreePropertyRef ObjectiveWidgetGuid;

	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree that we want to update with time.
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid", EditCondition="bUpdateSubObjective", Optional), Category="WarwolfMissionSystem")
	FStateTreePropertyRef SubObjectiveWidgetGuid;

};

/**
 * A task that counts up in seconds on tick until we reach a final value. Useful for timed states.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Timer"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfTimerTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfTimerTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfTimerTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Time");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR

	// @brief Current time on the timer.
	UPROPERTY()
	mutable float CurrentTime = 0;

	// @brief Time we stop counting at and succeed the task.
	UPROPERTY()
	mutable float FinishTime = 0;

	// @brief Have we finished counting?
	UPROPERTY()
	mutable bool bFinishedCounting = false;

	// @brief Guid of the current objective widget.
	UPROPERTY()
	mutable FGuid ObjGuid = FGuid();

	// @brief Guid of the current sub-objective widget this task is running on.
	UPROPERTY()
	mutable FGuid SubObjGuid = FGuid();

	// @brief World from task context.
	UPROPERTY()
	mutable TObjectPtr<UWorld> ContextWorld = nullptr;

	// @brief Are we counting down?
	UPROPERTY()
	mutable bool bCountDown = false;

	// @brief Should we be updating a widget with this count?
	UPROPERTY()
	mutable bool bUpdateWidget = false;
	
};
