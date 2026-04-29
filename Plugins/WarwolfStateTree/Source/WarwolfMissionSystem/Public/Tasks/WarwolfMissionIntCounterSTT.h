// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionIntCounterSTT.generated.h"

class UWarwolfMissionIntCounterTask;
USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfIntCounterTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief When we search for actors we should be counting, which gameplay tag should they also have?
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FGameplayTagContainer TagEventsToCount;

	// @brief How many counts do we succeed this task at?
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	int32 SucceedAtNum = 1;

	// @brief What class of actor should we look for when wanting to bind to counting events?
	// @warning Preferably not AActor as it'll get expensive!
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	TSubclassOf<AActor> ActorClassToFind;

	// @brief Do we want to update a sub-objective when something is counted?
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	bool bUpdateSubObjective = false;

	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree that we want to update with counts.
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid", EditCondition="bUpdateSubObjective", Optional), Category="WarwolfMissionSystem")
	FStateTreePropertyRef ObjectiveWidgetGuid;

	// @brief Reference to the ObjectiveWidgetGuid parameter in the state tree that we want to update with counts.
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/CoreUObject.Guid", EditCondition="bUpdateSubObjective", Optional), Category="WarwolfMissionSystem")
	FStateTreePropertyRef SubObjectiveWidgetGuid;

};

/**
 * A task that counts an int whenever FOnWarwolfIntCount is broadcast from actors with given tag.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Count Gameplay Events (int)"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfIntCounterTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfIntCounterTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfIntCounterTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

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

	// @brief Increment the counter int when called.
	void CountInt() const;

	// @brief Amount of events we've counted.
	UPROPERTY()
	mutable int32 CountedEvents = 0;

	// @brief Amount of events to count to before succeeding.
	UPROPERTY()
	mutable int32 SucceedAtNum = 0;

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

	// @brief Should we be updating a widget with this count?
	UPROPERTY()
	mutable bool bUpdateWidget = false;
	
};
