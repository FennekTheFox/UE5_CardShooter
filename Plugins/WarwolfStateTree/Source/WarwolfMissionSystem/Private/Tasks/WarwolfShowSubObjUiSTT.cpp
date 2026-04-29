// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfShowSubObjUiSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "WarwolfMissionUiInterface.h"
#include "Engine/World.h"
#include "StateTreePropertyRef.h"
#include "GameFramework/GameStateBase.h"


EStateTreeRunStatus FWarwolfShowSubObjectiveUiTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to show sub-objective on HUD."), *FString(__FUNCTION__));
	
	FWarwolfShowSubObjectiveUiTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfShowSubObjectiveUiTask>(*this);

	InstanceData.WidgetGuid = FGuid::NewGuid(); // We assume one hasn't been assigned.
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Setting sub-obj guid as %s."), *FString(__FUNCTION__), *InstanceData.WidgetGuid.ToString());

	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	const FGuid* BoundGuid = InstanceData.ObjectiveWidgetGuid.GetMutablePtr<FGuid>(Context);
	if (!BoundGuid)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundGuid is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	if(InstanceData.bHasParamOnStateTree)
	{
		FGuid* SubObjGuid = InstanceData.SubObjectiveWidgetGuid.GetMutablePtr<FGuid>(Context);
		if (!SubObjGuid)
		{
			UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s SubObjGuid is invalid. Returning fail state."), *FString(__FUNCTION__));
			return EStateTreeRunStatus::Failed;
		}

		*SubObjGuid = InstanceData.WidgetGuid;
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
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GameState is not valid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(GS);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s InterfaceInstance is not valid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	const FText SubObjectiveText = InstanceData.SubObjectiveText;
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Adding sub-objective to widget with guid: %s for sub-objective '%s' with guid: %s."), *FString(__FUNCTION__), *BoundGuid->ToString(), *SubObjectiveText.ToString(), *InstanceData.WidgetGuid.ToString());

	InterfaceInstance->Execute_ShowSubObjective(GS, SubObjectiveText, *BoundGuid, InstanceData.WidgetGuid);
	
	return EStateTreeRunStatus::Running;
}

void FWarwolfShowSubObjectiveUiTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to hide sub-objective on HUD."), *FString(__FUNCTION__));

	FWarwolfShowSubObjectiveUiTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfShowSubObjectiveUiTask>(*this);

	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("Current execution frame is invalid. Returning."));
		return;
	}
	
	const FGuid* BoundGuid = InstanceData.ObjectiveWidgetGuid.GetMutablePtr<FGuid>(Context);
	if (!BoundGuid)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundGuid is invalid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const UWorld* World = Context.GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is invalid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	AGameStateBase* GS = World->GetGameState();
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

	const FText SubObjectiveText = InstanceData.SubObjectiveText;
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Hiding sub-objective from widget with guid: %s for sub-objective '%s' with guid: %s."), *FString(__FUNCTION__), *BoundGuid->ToString(), *SubObjectiveText.ToString(), *InstanceData.WidgetGuid.ToString());
	
	InterfaceInstance->Execute_HideSubObjective(GS, *BoundGuid, InstanceData.WidgetGuid);
}
