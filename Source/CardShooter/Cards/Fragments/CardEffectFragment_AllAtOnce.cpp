#include "CardEffectFragment_AllAtOnce.h"

#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardEffects"

void UCardExecutionInstance_AllAtOnce::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_AllAtOnce* AsAllAtOnce = Cast<UCardEffectFragment_Execution_AllAtOnce>(Exec);

	if (!ensure(AsAllAtOnce))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	bool bValidExecutionFound = false;

	for (UCardEffectFragment_Execution* ExecIt : AsAllAtOnce->Executions)
	{
		if (ensure(ExecIt))
		{
			bValidExecutionFound = true;

			UCardExecutionInstance* NewInstance = ExecIt->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);
			NewInstance->OnExecutionCompleted.AddLambda([this, NewInstance](APawn* P, const FCardExecution_AdditionalData& A, UCardEffectFragment_Execution* E) {
				RunningSubInstances.Remove(NewInstance);
				if (RunningSubInstances.IsEmpty())
				{
					OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
				}
				});
			RunningSubInstances.Add(NewInstance);
		}
	}

	if (!bValidExecutionFound)
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
	}
}

UCardExecutionInstance* UCardEffectFragment_Execution_AllAtOnce::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance_AllAtOnce>(this);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
}

FText UCardEffectFragment_Execution_AllAtOnce::GetLocalizableDescription_Implementation()
{
	TArray<FText> ExecDescs;

	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		if (IsValid(Exec))
		{
			ExecDescs.Add(Exec->GetLocalizableDescription());
		}
	}

	return FText::Join(FText::FromString(" "), ExecDescs);
}

#undef LOCTEXT_NAMESPACE
