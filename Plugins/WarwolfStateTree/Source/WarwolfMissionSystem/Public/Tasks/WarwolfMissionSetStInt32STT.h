// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreePropertyRef.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "WarwolfMissionSetStInt32STT.generated.h"

USTRUCT()
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetInt32TaskInstanceData
{
	GENERATED_BODY()

	// @brief Reference to the state tree int32 parameter.
	UPROPERTY(EditAnywhere, meta = (RefType = "int32", Required), Category="WarwolfMissionSystem")
	FStateTreePropertyRef IntToSet;

	// @brief What to set the int as.
	UPROPERTY(EditAnywhere, meta = (Required), Category="WarwolfMissionSystem")
	int32 NewValue = 0;

};

/**
 * A task that sets an Int32 in the state tree parameters.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Set State Tree Param (int32)"))
struct WARWOLFMISSIONSYSTEM_API FWarwolfSetInt32Task : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

public:
	
	using FInstanceDataType = FWarwolfSetInt32TaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FWarwolfSetInt32TaskInstanceData::StaticStruct(); }
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