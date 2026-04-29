#include "CardEffectFragment_ComboSequence.h"

#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardEffects"


void UCardEffectFragment_Execution_ComboSequence::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	if (Executions.Num() == 1)
	{
		if (UCardEffectFragment_Execution* Exec = Executions[0])
		{
			Exec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);
		}

		return;
	}

	if (!ensure(AdditionalData.ComboIndex >= 0 && AdditionalData.ComboIndex < Executions.Num()))
	{
		return;
	}

	if (UCardEffectFragment_Execution* Exec = Executions[AdditionalData.ComboIndex])
	{
		Exec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);
	}
}

FText UCardEffectFragment_Execution_ComboSequence::GetLocalizableDescription_Implementation()
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
					Exec->GetLocalizableDescription()));
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
				Exec->GetLocalizableDescription()));
		}
		Count++;
	}

	return FText::Join(FText::FromString("\n"), ExecDescs);
}

#undef LOCTEXT_NAMESPACE