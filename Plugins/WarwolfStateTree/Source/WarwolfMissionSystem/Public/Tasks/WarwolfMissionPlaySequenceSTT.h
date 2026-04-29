// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionPlaySequenceSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfPlaySequenceTaskInstanceData
{
	GENERATED_BODY()

	// @brief What sequence do we want to play?
	UPROPERTY(EditAnywhere, meta = (Required), Category="WarwolfMissionSystem")
	TObjectPtr<ULevelSequence> SequenceToPlay = nullptr;

	// @brief Should we succeed the task when the sequence is complete? If true, we wait to succeed the task. If false, treat this task as a fire and forget to play a sequence.
	UPROPERTY(EditAnywhere, meta = (Required), Category="WarwolfMissionSystem")
	bool bWaitUntilSequenceFinished = false;
	
};

/**
 * A task that interface calls to the game state asking it to play a sequence.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Play Level Sequence"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfPlaySequenceTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfPlaySequenceTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfPlaySequenceTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Animation");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR
	
	void LevelSequenceFinished() const;

	// @brief Has the sequence finished?
	UPROPERTY()
	mutable bool bSequenceFinished = false;
	
};