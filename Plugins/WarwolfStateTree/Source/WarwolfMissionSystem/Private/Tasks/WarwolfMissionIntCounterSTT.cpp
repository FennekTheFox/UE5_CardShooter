// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionIntCounterSTT.h"
#include "WarwolfMissionTagComponent.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfMissionUiInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Tasks/WarwolfMissionTaskInterface.h"
#include "GameFramework/GameStateBase.h"


EStateTreeRunStatus FWarwolfIntCounterTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to count gameplay events."), *FString(__FUNCTION__));

    const FWarwolfIntCounterTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfIntCounterTask>(*this);
    SucceedAtNum = InstanceData.SucceedAtNum;
    const TSubclassOf<AActor> ActorClassToFind = InstanceData.ActorClassToFind;
    const FGameplayTagContainer TagsToFind = InstanceData.TagEventsToCount;
    ContextWorld = Context.GetWorld();
    bUpdateWidget = InstanceData.bUpdateSubObjective;
    bFinishedCounting = false;

    if (!ContextWorld)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s ContextWorld is invalid. Returning fail state."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }

    if (bUpdateWidget)
    {
        const FGuid* ObjGuidPtr = InstanceData.ObjectiveWidgetGuid.GetMutablePtr<FGuid>(Context);
        if (!ObjGuidPtr)
        {
            UE_LOG(LogWarwolfMissionSystem, Warning, TEXT("%s SubObjectiveWidgetGuid is invalid. Will try again next tick."), *FString(__FUNCTION__));
            return EStateTreeRunStatus::Running;
        }
        ObjGuid = *ObjGuidPtr;
        
        const FGuid* SubObjGuidPtr = InstanceData.SubObjectiveWidgetGuid.GetMutablePtr<FGuid>(Context);
        if (!SubObjGuidPtr)
        {
            UE_LOG(LogWarwolfMissionSystem, Warning, TEXT("%s SubObjectiveWidgetGuid is invalid. Will try again next tick."), *FString(__FUNCTION__));
            return EStateTreeRunStatus::Running;
        }
        SubObjGuid = *SubObjGuidPtr;

        if (!ObjGuid.IsValid() || !SubObjGuid.IsValid())
        {
            UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s ObjGuid or SubObjGuid aren't valid guids. Returning fail state."), *FString(__FUNCTION__));
            return EStateTreeRunStatus::Failed;
        }
    }
    
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(ContextWorld, ActorClassToFind, AllActors);
    for (AActor* Actor : AllActors)
    {
        if (!Actor) continue;
        
        UWarwolfMissionTagComponent* TagComp = Actor->FindComponentByClass<UWarwolfMissionTagComponent>();
        if (!TagComp || !TagComp->GetMissionTags().HasAnyExact(TagsToFind)) continue;

        TScriptInterface<IWarwolfMissionTaskInterface> InterfaceInstance = Actor;
        if (!InterfaceInstance)
        {
            UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Doesn't look like Actor %s implements IWarwolfMissionTaskInterface. Continuing."), *FString(__FUNCTION__), *Actor->GetName());
            continue;
        }

        FOnWarwolfIntCount& CounterDelegate = InterfaceInstance->GetIntCountDelegate();
        CounterDelegate.AddRaw(this, &FWarwolfIntCounterTask::CountInt);
        UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Binding delegate from: %s."), *FString(__FUNCTION__), *Actor->GetName());
    }
    
    CountedEvents = -1; // Start at -1 as we call CountInt when we first enter the state to update the UI.
    CountInt();
    
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FWarwolfIntCounterTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    if (bFinishedCounting) return EStateTreeRunStatus::Succeeded;

    return EStateTreeRunStatus::Running;
}


void FWarwolfIntCounterTask::CountInt() const
{
    if (bFinishedCounting) return;
    
    CountedEvents++;

    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Counted: %d."), *FString(__FUNCTION__), CountedEvents);
    
    if (!ContextWorld)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is invalid. Returning."), *FString(__FUNCTION__));
        return;
    }
	
    AGameStateBase* GS = ContextWorld->GetGameState();
    if (!GS)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GameState is not valid. Returning."), *FString(__FUNCTION__));
        return;
    }

    const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(GS);
    if (!InterfaceInstance)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
        return;
    }

    if (bUpdateWidget) InterfaceInstance->Execute_UpdateSubObjective_IntCount(GS, ObjGuid, SubObjGuid, CountedEvents, SucceedAtNum);

    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Updating count on UI to: (%d/%d)."), *FString(__FUNCTION__), CountedEvents, SucceedAtNum);
    
    if (CountedEvents < SucceedAtNum) return;

    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Successfully counted enough events(%d/%d)."), *FString(__FUNCTION__), CountedEvents, SucceedAtNum);

    bFinishedCounting = true;
}
