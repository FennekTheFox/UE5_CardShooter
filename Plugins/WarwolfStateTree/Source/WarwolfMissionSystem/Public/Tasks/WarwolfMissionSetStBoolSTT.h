// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionSetStBoolSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetBoolTaskInstanceData
{
	GENERATED_BODY()

	// @brief Reference to the state tree bool parameter.
	UPROPERTY(EditAnywhere, meta = (RefType = "bool", Required), Category="WarwolfMissionSystem")
	FStateTreePropertyRef BoolToSet;

	// @brief What to set the bool as.
	UPROPERTY(EditAnywhere, meta = (Required), Category="WarwolfMissionSystem")
	bool bNewValue = false;

};

/**
 * A task that sets a bool in the state tree parameters.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Set State Tree Param (bool)"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetBoolTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfSetBoolTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfSetBoolTaskInstanceData::StaticStruct(); }
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