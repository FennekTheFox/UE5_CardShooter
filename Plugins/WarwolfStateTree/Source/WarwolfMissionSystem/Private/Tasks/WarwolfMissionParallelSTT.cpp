// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Tasks/WarwolfMissionParallelSTT.h"

#include "WarwolfMissionDataAsset.h"

// Fix redefined namespace error on android.
#ifdef LOCTEXT_NAMESPACE
#undef LOCTEXT_NAMESPACE
#endif // LOCTEXT_NAMESPACE
#define LOCTEXT_NAMESPACE "StateTree"

FWarwolfRunParallelTask::FWarwolfRunParallelTask()
{
    bShouldCopyBoundPropertiesOnTick = true;
    bShouldCopyBoundPropertiesOnExitState = true;
}

EStateTreeRunStatus FWarwolfRunParallelTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	const EStateTreeRunStatus SuperRunStatus = Super::Tick(Context, DeltaTime);
	if (SuperRunStatus == EStateTreeRunStatus::Failed) return SuperRunStatus;

    const FWarwolfRunParallelTaskInstanceData& InstanceData = Context.GetInstanceData<FWarwolfRunParallelTaskInstanceData>(*this);
    if (InstanceData.StatusParamName.IsNone())
    {
        // If StatusParamName is invalid, we're always going to end up failing. Just return a fail state so we don't
        // keep ticking the task.
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s Status param name is NONE. Returning fail state."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }
    
    const FStateTreeExecutionFrame* CurrentlyProcessedFrame = Context.GetCurrentlyProcessedFrame();
    if (!CurrentlyProcessedFrame)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: CurrentlyProcessedFrame is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }

    const FStateTreeInstanceData SubInstanceData = InstanceData.TreeInstanceData;
    const FConstStructView GlobalParameters = SubInstanceData.GetStorage().GetGlobalParameters();
    
    if (!GlobalParameters.IsValid())
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: GlobalParameters is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }

    const UScriptStruct* ParamScript = GlobalParameters.GetScriptStruct();
    if (!ParamScript)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: ParamScript is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }

    /*
    for (TFieldIterator<FProperty> It(ParamScript); It; ++It)
    {
        FProperty* Property = *It;
        UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Found field: %s of type: %s"), *InstanceData.StateTree.GetStateTree()->GetName(), *Property->GetName(), *Property->GetCPPType());
    }
    */

    FProperty* FoundEnum = ParamScript->FindPropertyByName(InstanceData.StatusParamName);
    if (!FoundEnum)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: FoundEnum is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }

    const FEnumProperty* EnumProperty = CastField<FEnumProperty>(FoundEnum);
    if (!EnumProperty)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: EnumProperty is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }

    UEnum* EnumPtr = EnumProperty->GetEnum();
    if (!EnumPtr)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: EnumPtr is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }
    
    const void* StructData = GlobalParameters.GetMemory();
    if (!StructData)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: StructData is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }
    
    const uint8* ValuePtr = EnumProperty->ContainerPtrToValuePtr<uint8>(StructData);
    if (!ValuePtr)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s: ValuePtr is not valid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }
    
    EWarwolfMissionStatus StatusEnum = static_cast<EWarwolfMissionStatus>(*ValuePtr);
    EWarwolfMissionStatus* BoundStatus = InstanceData.StatusToSet.GetMutablePtr<EWarwolfMissionStatus>(Context);
    if (!BoundStatus)
    {
        UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s BoundStatus is invalid."), *FString(__FUNCTION__));
        return EStateTreeRunStatus::Running;
    }
    
    if (StatusEnum == *BoundStatus) return SuperRunStatus; // No point setting it to the same value, or logging it.

    const FString BoundStatusAsString = UEnum::GetValueAsString(*BoundStatus);
    const FString NewValueAsString = UEnum::GetValueAsString(StatusEnum);
    UE_LOG(LogWarwolfMissionSystem, Log, TEXT("%s Setting objective status from '%s' to '%s'."), *FString(__FUNCTION__), *BoundStatusAsString, *NewValueAsString);

    *BoundStatus = StatusEnum;
    
    return SuperRunStatus;
}

#if WITH_EDITOR

FText FWarwolfRunParallelTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
    const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
    check(InstanceData);

    FText StateTreeValue = BindingLookup.GetBindingSourceDisplayName(FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, StateTree)), Formatting);
    if (StateTreeValue.IsEmpty())
    {
        StateTreeValue = FText::FromString(GetNameSafe(InstanceData->StateTree.GetStateTree()));
    }

    const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
        ? LOCTEXT("WarwolfRunParallelRich", "<b>Warwolf Run Parallel</> {Asset}")
        : LOCTEXT("WarwolfRunParallel", "Warwolf Run Parallel {Asset}");

    return FText::FormatNamed(Format,
        TEXT("Asset"), StateTreeValue);
}

#endif //WITH_EDITOR
