// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionSetStBoolSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyRef.h"


EStateTreeRunStatus FWarwolfSetBoolTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to set a state tree bool."), *FString(__FUNCTION__));
	
	const FWarwolfSetBoolTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfSetBoolTask>(*this);
	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	bool* BoundBool = InstanceData.BoolToSet.GetMutablePtr<bool>(Context);
	if (!BoundBool)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundBool is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const FString BoundBoolAsString = *BoundBool ? TEXT("True") : TEXT("False");
	const FString NewValueAsString = InstanceData.bNewValue ? TEXT("True") : TEXT("False");
	
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Setting bool from '%s' to '%s'."), *FString(__FUNCTION__), *BoundBoolAsString, *NewValueAsString);

	*BoundBool = InstanceData.bNewValue;
	
	return EStateTreeRunStatus::Running;
}
