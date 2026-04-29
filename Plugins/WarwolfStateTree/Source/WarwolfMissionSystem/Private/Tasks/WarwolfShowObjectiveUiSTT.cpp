// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfShowObjectiveUiSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "WarwolfMissionUiInterface.h"
#include "Engine/World.h"
#include "StateTreePropertyRef.h"
#include "WarwolfMissionDataAsset.h"
#include "GameFramework/GameStateBase.h"


EStateTreeRunStatus FWarwolfShowObjectiveUiTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to show objective on HUD."), *FString(__FUNCTION__));
	
	FWarwolfShowObjectiveUiTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfShowObjectiveUiTask>(*this);
	const UWarwolfMissionDataAsset* CurrentMissionData = InstanceData.MissionData;

	if (!IsValid(CurrentMissionData))
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s CurrentMissionData is not valid. Failing state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	FGuid* BoundGuid = InstanceData.ObjectiveWidgetGuid.GetMutablePtr<FGuid>(Context);
	if (!BoundGuid)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundGuid is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Running;
	}
	*BoundGuid = FGuid::NewGuid();

	const UWorld* World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is invalid. Returning fail state."), *FString(__FUNCTION__));
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
	
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s New GUID set for BoundGuid: %s for objective '%s'."), *FString(__FUNCTION__), *BoundGuid->ToString(), *InstanceData.ObjectiveText.ToString());

	InterfaceInstance->Execute_ShowObjective(GS, InstanceData.ObjectiveText, *BoundGuid, CurrentMissionData->MissionType);
	
	return EStateTreeRunStatus::Running;
}
