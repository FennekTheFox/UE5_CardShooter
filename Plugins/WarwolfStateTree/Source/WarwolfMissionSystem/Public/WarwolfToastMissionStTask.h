// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
//
#include "WarwolfMissionDataAsset.h"
#include "StateTreeLinker.h"
#include "StateTreeExecutionTypes.h"
#include "WarwolfMissionTypes.h"
//
#include "WarwolfToastMissionStTask.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfToastMissionStartTaskInstanceData
{
	GENERATED_BODY()
	
	// @brief The mission data from the state tree parameters.
	UPROPERTY(EditAnywhere, Category = "Input", meta=(Required))
	TObjectPtr<UWarwolfMissionDataAsset> MissionData = nullptr;

	// @brief The type of toast we want to display.
	UPROPERTY(EditAnywhere, Category="WarwolfMissionSystem")
	EWarwolfMissionToastType ToastType = EWarwolfMissionToastType::MissionStart;
	
};

/**
 * A task that toasts a mission on the UI.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Toast Mission Task"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfToastMissionStartTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfToastMissionStartTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfToastMissionStartTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Text");
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