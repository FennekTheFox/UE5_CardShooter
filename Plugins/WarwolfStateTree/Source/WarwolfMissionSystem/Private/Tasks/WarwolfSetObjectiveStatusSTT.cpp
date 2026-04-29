// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfSetObjectiveStatusSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyRef.h"


EStateTreeRunStatus FWarwolfSetObjStatusTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to set a state tree objective status."), *FString(__FUNCTION__));
	
	const FWarwolfSetObjStatusTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfSetObjStatusTask>(*this);
	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	EWarwolfMissionStatus* BoundStatus = InstanceData.StatusToSet.GetMutablePtr<EWarwolfMissionStatus>(Context);
	if (!BoundStatus)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundStatus is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const FString BoundStatusAsString = UEnum::GetValueAsString(*BoundStatus);
	const FString NewValueAsString = UEnum::GetValueAsString(InstanceData.NewValue);
	UE_LOG(LogWarwolfMissionSystem, Verbose, TEXT("%s Setting objective status from '%s' to '%s'."), *FString(__FUNCTION__), *BoundStatusAsString, *NewValueAsString);

	*BoundStatus = InstanceData.NewValue;

	const FString NewBoundStatusAsString = UEnum::GetValueAsString(*BoundStatus);
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Confirming status is now '%s'."), *FString(__FUNCTION__), *NewBoundStatusAsString);
	
	return EStateTreeRunStatus::Running;
}
