// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "WarwolfMissionManager.h"

#include "StateTree.h"
#include "WarwolfMissionTypes.h"

void UWarwolfMissionManager::SendMissionEvent_Implementation(const FStateTreeEvent& Event)
{
	if(!IsRunning())
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Trying to send an event when the state tree isn't running. Returning."), *FString(__FUNCTION__));
		return;
	}

	if(!IsValid(StateTreeRef.GetStateTree()))
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Trying to send an event when the state tree isn't valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Sending state tree event %s to state tree: %s"), *FString(__FUNCTION__), *Event.Tag.ToString(), *StateTreeRef.GetStateTree()->GetName());
	
	SendStateTreeEvent(Event);
}

void UWarwolfMissionManager::SetMissionStateTree(UStateTree* InStateTree)
{
	StateTreeRef.SetStateTree(InStateTree);
}
