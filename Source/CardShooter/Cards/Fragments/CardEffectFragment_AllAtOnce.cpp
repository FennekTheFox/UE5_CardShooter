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

			UCardExecutionInstance* NewInstance = ExecIt->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData, Exec);
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

FText UCardEffectFragment_Execution_AllAtOnce::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	TArray<FText> ExecDescs;

	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		if (IsValid(Exec))
		{
			ExecDescs.Add(Exec->GetLocalizableDescription(CardIdentity));
		}
	}

	return FText::Join(FText::FromString(" "), ExecDescs);
}

UCardEffectFragment_Execution_AllAtOnce::UCardEffectFragment_Execution_AllAtOnce()
{
	ExecInstanceClass = UCardExecutionInstance_AllAtOnce::StaticClass();
}

#undef LOCTEXT_NAMESPACE
