// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionSetStInt32STT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyRef.h"


EStateTreeRunStatus FWarwolfSetInt32Task::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to set a state tree int32."), *FString(__FUNCTION__));
	
	const FWarwolfSetInt32TaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfSetInt32Task>(*this);
	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	int32* BoundInt32 = InstanceData.IntToSet.GetMutablePtr<int32>(Context);
	if (!BoundInt32)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundInt32 is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	UE_LOG(LogWarwolfMissionSystem, Verbose, TEXT("%s Setting int32 from '%d' to '%d'."), *FString(__FUNCTION__), *BoundInt32, InstanceData.NewValue);

	*BoundInt32 = InstanceData.NewValue;
	
	return EStateTreeRunStatus::Running;
}
