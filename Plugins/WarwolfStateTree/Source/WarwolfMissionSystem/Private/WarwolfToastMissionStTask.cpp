// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "WarwolfToastMissionStTask.h"

#include "WarwolfMissionTypes.h"
#include "WarwolfMissionUiInterface.h"
#include "StateTreeExecutionContext.h"
#include "Engine/World.h"
#include "GameFramework/GameState.h"

EStateTreeRunStatus FWarwolfToastMissionStartTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to toast a mission."), *FString(__FUNCTION__));

    const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    const UWarwolfMissionDataAsset* CurrentMissionData = InstanceData.MissionData;
    const EWarwolfMissionToastType ToastType = InstanceData.ToastType;

    if (!IsValid(CurrentMissionData))
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s CurrentMissionData is not valid. Failing state."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }
    
    World = Context.GetWorld();
    if (!World)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s ContextWorld is invalid. Returning fail state."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }

    AGameStateBase* GS = World->GetGameState();
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

    const FText MissionName = CurrentMissionData->MissionName;
    const EWarwolfMissionType MissionType = CurrentMissionData->MissionType;
    const FString MissionTypeString = UEnum::GetDisplayValueAsText(MissionType).ToString();
    const FString ToastTypeString = UEnum::GetDisplayValueAsText(ToastType).ToString();

    UE_LOG(LogWarwolfMissionSystem
        , Log
        , TEXT("%s Toasting mission %s with name %s and type %s")
        , *FString(__FUNCTION__)
        , *ToastTypeString
        , *MissionName.ToString()
        , *MissionTypeString
        );

    InterfaceInstance->Execute_ToastMission(GS, MissionName, MissionType, ToastType);

    return FStateTreeTaskCommonBase::EnterState(Context, Transition);
}
