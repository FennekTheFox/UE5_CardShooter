#include "GameplayEffect_CardExecution.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"

UGameplayEffect_CardExecution::UGameplayEffect_CardExecution()
{

}

FText UGameplayEffect_CardExecution::GetDescription(UCardEffectFragment_ApplyEffectToOwner* OwningCEF_AETO)
{
	FFormatNamedArguments Args;
	OwningCEF_AETO->GetSetByCallerFormattingArgs(Args);

	return FText::Format(GetDescription(), Args);
}