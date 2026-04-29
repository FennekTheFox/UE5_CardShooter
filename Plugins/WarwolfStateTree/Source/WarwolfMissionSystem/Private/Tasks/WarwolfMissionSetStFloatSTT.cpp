// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionSetStFloatSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyRef.h"


EStateTreeRunStatus FWarwolfSetFloatTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to set a state tree float."), *FString(__FUNCTION__));
	
	const FWarwolfSetFloatTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfSetFloatTask>(*this);
	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	float* BoundFloat = InstanceData.FloatToSet.GetMutablePtr<float>(Context);
	if (!BoundFloat)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundFloat is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const FString BoundFloatAsString = FString::SanitizeFloat(*BoundFloat);
	const FString NewValueAsString = FString::SanitizeFloat(InstanceData.NewValue);
	
	UE_LOG(LogWarwolfMissionSystem, Verbose, TEXT("%s Setting float from '%s' to '%s'."), *FString(__FUNCTION__), *BoundFloatAsString, *NewValueAsString);

	*BoundFloat = InstanceData.NewValue;
	
	return EStateTreeRunStatus::Running;
}
