#include "GameplayEffect_CardExecution.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"

UGameplayEffect_CardExecution::UGameplayEffect_CardExecution()
{

}

FText UGameplayEffect_CardExecution::GetDescription(const FCardIdentity& CardIdentity, UCardEffectFragment_ApplyEffectToOwner* OwningCEF_AETO)
{
	FFormatNamedArguments Args;
	OwningCEF_AETO->GetSetByCallerFormattingArgs(Args, CardIdentity);

	return FText::Format(GetDescription(CardIdentity), Args);
}