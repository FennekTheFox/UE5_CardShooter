// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionHideObjWidgetSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "WarwolfMissionUiInterface.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "StateTreePropertyRef.h"


EStateTreeRunStatus FWarwolfHideObjectiveUiTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to hide objective on HUD."), *FString(__FUNCTION__));
	
	FWarwolfHideObjectiveUiTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfHideObjectiveUiTask>(*this);

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

	const UWorld* World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	AGameStateBase* GS = World->GetGameState();
	if (!GS)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GameState is not valid. Returning."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(GS);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Using Guid: %s to hide objective."), *FString(__FUNCTION__), *BoundGuid->ToString());

	InterfaceInstance->Execute_HideObjective(GS, *BoundGuid);
	
	return EStateTreeRunStatus::Running;
}
