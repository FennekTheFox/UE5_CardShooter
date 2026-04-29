// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionPlaySequenceSTT.h"

#include "WarwolfMissionTypes.h"
#include "StateTreeExecutionContext.h"
#include "Tasks/WarwolfMissionTaskInterface.h"
#include "GameFramework/GameStateBase.h"
#include "Tasks/WarwolfMissionTaskHelper.h"


EStateTreeRunStatus FWarwolfPlaySequenceTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Task running to play a level sequence."), *FString(__FUNCTION__));

	bSequenceFinished = false;
	
	const FWarwolfPlaySequenceTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfPlaySequenceTask>(*this);
	const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
	if (!CurrentlyProcessedFrame)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}
	
	ULevelSequence* BoundSequence = InstanceData.SequenceToPlay;
	if (!BoundSequence)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundSequenced is invalid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	AGameStateBase* GS = Context.GetWorld()->GetGameState();
	if (!GS)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GameState is not valid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const IWarwolfMissionTaskInterface* InterfaceInstance = Cast<IWarwolfMissionTaskInterface>(GS);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s InterfaceInstance is not valid. Returning fail state."), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Failed;
	}

	const FString DebugString = InstanceData.bWaitUntilSequenceFinished ? TEXT("Will") : TEXT("Will not");
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Playing level sequence. %s wait until the sequence is finished."), *FString(__FUNCTION__), *DebugString);

	ULevelSequencePlayer* SequencePlayer = InterfaceInstance->Execute_PlayLevelSequence(GS, BoundSequence);

	if (InstanceData.bWaitUntilSequenceFinished)
	{
		UWarwolfMissionTaskHelper* DelegateHelper = NewObject<UWarwolfMissionTaskHelper>();
		DelegateHelper->TaskInstance = this;
		SequencePlayer->OnFinished.AddDynamic(DelegateHelper, &UWarwolfMissionTaskHelper::OnLevelSequenceFinished);
		
		return EStateTreeRunStatus::Running;
	}

	return EStateTreeRunStatus::Succeeded;
}

EStateTreeRunStatus FWarwolfPlaySequenceTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	if (bSequenceFinished)
	{
		UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Play sequence task will now succeed!"), *FString(__FUNCTION__));
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void FWarwolfPlaySequenceTask::LevelSequenceFinished() const
{
	UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Level sequence finished."), *FString(__FUNCTION__));
	bSequenceFinished = true;
}
