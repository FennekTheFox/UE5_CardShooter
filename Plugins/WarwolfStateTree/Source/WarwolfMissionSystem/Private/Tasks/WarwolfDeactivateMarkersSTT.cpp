// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfDeactivateMarkersSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "WarwolfMissionMarker.h"
#include "Kismet/GameplayStatics.h"


EStateTreeRunStatus FWarwolfDeactivateMarkersWithTagsTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to find and deactivate markers with tags."), *FString(__FUNCTION__));

	int32 AmountOfTagsFound = 0;
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const FGameplayTagContainer TagsToFind = InstanceData.TagsToFind;

	World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s ContextWorld is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AWarwolfMissionMarker::StaticClass(), FoundActors);
    
	for (AActor* Actor : FoundActors)
	{
		AWarwolfMissionMarker* Marker = Cast<AWarwolfMissionMarker>(Actor);
		if (!Marker)
		{
			continue;
		}

		UWarwolfMissionTagComponent* TagComp = Marker->FindComponentByClass<UWarwolfMissionTagComponent>();
		if (!TagComp)
		{
			const FString MarkerName = Marker->GetName();
			UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s We found a marker %s but it didn't have a gameplay tag component? This isn't good. Skipping."), *FString(__FUNCTION__), *MarkerName);
			continue;
		}

		if(!TagComp->GetMissionTags().HasAnyExact(TagsToFind))
		{
			continue;
		}

		Marker->DeactivateMarker();
		AmountOfTagsFound++;
	}

	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Found %d markers with specified tags."), *FString(__FUNCTION__), AmountOfTagsFound);

	return FStateTreeTaskCommonBase::EnterState(Context, Transition);
}
