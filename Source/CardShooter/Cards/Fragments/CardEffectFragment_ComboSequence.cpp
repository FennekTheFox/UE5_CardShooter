#include "CardEffectFragment_ComboSequence.h"

#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardEffects"


void UCardExecutionInstance_ComboSequence::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_ComboSequence* AsComboSeq = Cast<UCardEffectFragment_Execution_ComboSequence>(Exec);

	if (!ensure(AsComboSeq))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	if (AsComboSeq->Executions.Num() == 1)
	{
		if (UCardEffectFragment_Execution* ComboExec = AsComboSeq->Executions[0])
		{
			RunningSubInstances.Add(ComboExec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData, Exec));
		}

		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	if (!ensure(AdditionalData.ComboIndex >= 0 && AdditionalData.ComboIndex < AsComboSeq->Executions.Num()))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	if (UCardEffectFragment_Execution* ComboExec = AsComboSeq->Executions[AdditionalData.ComboIndex])
	{
		RunningSubInstances.Add(ComboExec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData, Exec));
	}
}

FText UCardEffectFragment_Execution_ComboSequence::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	if (Executions.IsEmpty())
	{
		return FText::GetEmpty();
	}

	TArray<FText> ExecDescs;
	if(Executions.Num() == 1)
	{
		UCardEffectFragment_Execution* Exec = Executions[0];

		if (IsValid(Exec))
		{
			for (int i = 1; i <= Executions.Num(); i++)
			{
				ExecDescs.Add(FText::Format(LOCTEXT("ComboSequencePartDesc", "{0}: {1}"),
					FText::AsNumber(i+1),
					Exec->GetLocalizableDescription(CardIdentity)));
			}
		}

		return FText::Join(FText::FromString("\n"), ExecDescs);
	}

	int32 Count = 1;
	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		if (IsValid(Exec))
		{
			ExecDescs.Add(FText::Format(LOCTEXT("ComboSequencePartDesc", "{0}: {1}"),
				FText::AsNumber(Count),
				Exec->GetLocalizableDescription(CardIdentity)));
		}
		Count++;
	}

	return FText::Join(FText::FromString("\n"), ExecDescs);
}

UCardEffectFragment_Execution_ComboSequence::UCardEffectFragment_Execution_ComboSequence()
{
	ExecInstanceClass = UCardEffectFragment_Execution_ComboSequence::StaticClass();
}

#undef LOCTEXT_NAMESPACE
