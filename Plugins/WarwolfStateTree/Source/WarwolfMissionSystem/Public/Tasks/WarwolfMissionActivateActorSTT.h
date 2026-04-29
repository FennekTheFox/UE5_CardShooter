// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionActivateActorSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfMissionActivateActorTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief Gameplay tags to look for.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	FGameplayTagContainer TagsToFind = FGameplayTagContainer();

	// @brief Class of actors to look for. Best to avoid AActor as it'll get expensive.
	UPROPERTY(EditAnywhere, meta=(Required), Category="WarwolfMissionSystem")
	TSubclassOf<AActor> ActorClassToFind;
	
};

/**
 * A task that looks for actors of the given class with given tags and activates them.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Activate All Actors With Tags"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfMissionActivateActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfMissionActivateActorTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfMissionActivateActorTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Find");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR

	// @brief World from task context.
	UPROPERTY()
	mutable TObjectPtr<UWorld> World = nullptr;
	
};