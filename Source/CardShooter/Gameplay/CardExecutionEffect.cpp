#include "CardExecutionEffect.h"
#include "Cards/CardEffectFragment_EffectToSelf.h"

FText UGameplayEffect_CardExecution::GetDescription(UCardEffectFragment_ApplyEffectToOwner* OwningCEF_AETO)
{
	FFormatNamedArguments Args;
	OwningCEF_AETO->GetSetByCallerFormattingArgs(Args);

	return FText::Format(GetDescription(), Args);
}