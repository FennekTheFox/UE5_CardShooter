#include "CardEffectFragment_AllAtOnce.h"

#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardEffects"


void UCardEffectFragment_Execution_AllAtOnce::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		Exec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);
	}
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