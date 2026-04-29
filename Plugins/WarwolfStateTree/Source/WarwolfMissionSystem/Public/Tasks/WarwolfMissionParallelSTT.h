// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "StateTreePropertyRef.h"
#include "StateTreeModule/Public/Tasks/StateTreeRunParallelStateTreeTask.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfMissionParallelSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfRunParallelTaskInstanceData : public FStateTreeRunParallelStateTreeTaskInstanceData
{
	GENERATED_BODY()

	// @brief Name of the state tree objective status parameter on the child state tree, e.g. 'CompletionStatus'.
	UPROPERTY(EditAnywhere, meta = (RefType = "FName", Required), Category="WarwolfMissionSystem")
	FName StatusParamName = NAME_None;

	// @brief Reference to the parent state tree parameter for this objective's status.
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/WarwolfMissionSystem.EWarwolfMissionStatus", Required), Category="WarwolfMissionSystem")
	FStateTreePropertyRef StatusToSet;

};

/**
 * Extension of the 'Run Parallel State Tree' task that also updates current objective status to a parameter in
 * state tree running this task.
 */
/*
 * TODO: Binding properties from the parallel task does not seem to update, even with bCopyBoundPropertiesOnTick
 * being true. Maybe I'm just misunderstanding things. This workaround is the only way I could think of to get
 * around the issue.
*/
USTRUCT(BlueprintType, meta = (DisplayName = "Warwolf Run Parallel State Tree"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfRunParallelTask : public FStateTreeRunParallelStateTreeTask
{
	GENERATED_BODY()

public:

	FWarwolfRunParallelTask();
	
	using FInstanceDataType = FWarwolfRunParallelTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfRunParallelTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR
	
};
