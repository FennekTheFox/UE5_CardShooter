// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfSetObjectiveStatusSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetObjStatusTaskInstanceData
{
	GENERATED_BODY()

	// @brief Reference to the state tree objective status parameter.
	UPROPERTY(EditAnywhere, meta = (RefType = "/Script/WarwolfMissionSystem.EWarwolfMissionStatus", Required), Category="WarwolfMissionSystem")
	FStateTreePropertyRef StatusToSet;

	// @brief What to set the status as.
	UPROPERTY(EditAnywhere, meta = (Required), Category="WarwolfMissionSystem")
	EWarwolfMissionStatus NewValue = EWarwolfMissionStatus::NotFinished;

};

/**
 * A task that sets a EWarwolfMissionStatus in the state tree parameters.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Set State Tree Param (Objective Status)"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetObjStatusTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfSetObjStatusTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfSetObjStatusTaskInstanceData::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.EnableDisable");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Red;
	}
#endif // WITH_EDITOR
	
};