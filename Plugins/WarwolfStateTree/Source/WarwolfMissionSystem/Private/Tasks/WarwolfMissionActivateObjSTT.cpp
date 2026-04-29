// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionActivateObjSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "WarwolfMissionInterface.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"


EStateTreeRunStatus FWarwolfActivateObjTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to activate secondary objectives."), *FString(__FUNCTION__));
	
	const UWorld* World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	AGameStateBase* GameState = World->GetGameState();
	if (GameState == nullptr)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GameState is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const IWarwolfMissionInterface* InterfaceInstance = Cast<IWarwolfMissionInterface>(GameState);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s InterfaceInstance is not valid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const FGameplayTagContainer MissionTags = InstanceData.MissionTags;

	const TArray<FGameplayTag> TagArray = MissionTags.GetGameplayTagArray();
	for (FGameplayTag Tag : TagArray)
	{
		const FString TagString = Tag.ToString();
		const FString StartLeafString = ".Start";
		const FString AppendedString = TagString + StartLeafString;
		const FName EventTagName = FName(AppendedString);
		const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(EventTagName);

		if (!EventTag.IsValid())
		{
			UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Couldn't find tag for %s. We've either hit a race condition with the tag dictionary, or the tag has been removed? Returning fail state."), *FString(__FUNCTION__), *AppendedString);
			return EStateTreeRunStatus::Failed;
		}
		
		const FName Origin = FName("WarwolfMissionActivateObjSTT");
		FStateTreeEvent StateTreeEvent;
		StateTreeEvent.Tag = EventTag;
		StateTreeEvent.Origin = Origin;

		InterfaceInstance->Execute_SendMissionEvent(GameState, StateTreeEvent);
	}

	return FStateTreeTaskCommonBase::EnterState(Context, Transition);
}
