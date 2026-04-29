// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionActivateActorSTT.h"

#include "Kismet/GameplayStatics.h"
#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "WarwolfMissionTagComponent.h"
#include "Tasks/WarwolfMissionTaskInterface.h"
#include "Engine/World.h"

EStateTreeRunStatus FWarwolfMissionActivateActorTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to find and activate actors with tags."), *FString(__FUNCTION__));

	int32 AmountOfTagsFound = 0;
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const FGameplayTagContainer TagsToFind = InstanceData.TagsToFind;
	const TSubclassOf<AActor> ActorClassToFind = InstanceData.ActorClassToFind;

	World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s ContextWorld is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ActorClassToFind, FoundActors);
    
	for (AActor* Actor : FoundActors)
	{
		if (!Actor)
		{
			continue;
		}

		UWarwolfMissionTagComponent* TagComp = Actor->FindComponentByClass<UWarwolfMissionTagComponent>();
		if (!TagComp)
		{
			continue;
		}

		if(!TagComp->GetMissionTags().HasAnyExact(TagsToFind))
		{
			continue;
		}

		if (!Actor->Implements<UWarwolfMissionTaskInterface>())
		{
			UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Doesn't look like Actor %s implements IWarwolfMissionTaskInterface. Continuing."), *FString(__FUNCTION__), *Actor->GetName());
			continue;
		}

		IWarwolfMissionTaskInterface::Execute_ActivateMissionActor(Actor);
		UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Activated actor '%s' with specified tags."), *FString(__FUNCTION__), *Actor->GetName());
		
		AmountOfTagsFound++;
	}

	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Activated %d actors with specified tags."), *FString(__FUNCTION__), AmountOfTagsFound);

	return EStateTreeRunStatus::Running;
}

