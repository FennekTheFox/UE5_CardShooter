// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionSetStFloatSTT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetFloatTaskInstanceData
{
	GENERATED_BODY()

	// @brief Reference to the state tree float parameter.
	UPROPERTY(EditAnywhere, meta = (RefType = "float", Required), Category="WarwolfMissionSystem")
	FStateTreePropertyRef FloatToSet;

	// @brief What to set the float as.
	UPROPERTY(EditAnywhere, meta = (Required), Category="WarwolfMissionSystem")
	float NewValue = 0.f;

};

/**
 * A task that sets a float in the state tree parameters.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Set State Tree Param (float)"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetFloatTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfSetFloatTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfSetFloatTaskInstanceData::StaticStruct(); }
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