// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionDeactivateActorSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfMissionDeactivateActorTaskInstanceData
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
 * A task that looks for actors of the given class with given tags and deactivates them.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Deactivate All Actors With Tags"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfMissionDeactivateActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfMissionDeactivateActorTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfMissionDeactivateActorTaskInstanceData::StaticStruct(); }
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