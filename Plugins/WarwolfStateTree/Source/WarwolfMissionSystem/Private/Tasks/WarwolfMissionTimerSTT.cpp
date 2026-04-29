// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionTimerSTT.h"
#include "WarwolfMissionUiInterface.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"


EStateTreeRunStatus FWarwolfTimerTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to run a timer."), *FString(__FUNCTION__));

    const FWarwolfTimerTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfTimerTask>(*this);
    FinishTime = InstanceData.FinalTime;
    ContextWorld = Context.GetWorld();
    bUpdateWidget = InstanceData.bUpdateSubObjective;
    bFinishedCounting = false;
    bCountDown = InstanceData.bCountDown;

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
    
    CurrentTime = 0.f;
    
    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FWarwolfTimerTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    if (bFinishedCounting) return EStateTreeRunStatus::Succeeded;
    
    CurrentTime += DeltaTime;
    
    UE_LOG(LogWarwolfMissionSystem, Verbose, TEXT("%s Current time: %f."), *FString(__FUNCTION__), CurrentTime);
    
    if (!ContextWorld)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is invalid. Returning fail."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }
	
    AGameStateBase* GS = ContextWorld->GetGameState();
    if (!GS)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GameState is not valid. Returning fail state."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }

    const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(GS);
    if (!InterfaceInstance)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s InterfaceInstance is not valid. Returning fail state."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }

    const float TimeToDisplay = (bCountDown) ? FinishTime - CurrentTime : CurrentTime;

    if (bUpdateWidget) InterfaceInstance->Execute_UpdateSubObjective_Timer(GS, ObjGuid, SubObjGuid, TimeToDisplay, FinishTime);
    
    if (CurrentTime < FinishTime) return EStateTreeRunStatus::Running;

    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Successfully counted to time (%s / %s)."), *FString(__FUNCTION__), *FString::SanitizeFloat(CurrentTime), *FString::SanitizeFloat(FinishTime));

    bFinishedCounting = true;

    return EStateTreeRunStatus::Succeeded;
}
