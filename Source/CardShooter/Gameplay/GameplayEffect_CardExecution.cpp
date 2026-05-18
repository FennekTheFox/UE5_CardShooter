#include "GameplayEffect_CardExecution.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"
#include "Util/CardShooterUtility.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"

UGameplayEffect_CardExecution::UGameplayEffect_CardExecution()
{

}

FText UGameplayEffect_CardExecution::GetDescription(const FCardIdentity& CardIdentity, UCardEffectFragment_Execution_WithParameters* OwningCEF_E_WA)
{
	FFormatNamedArguments Args;
	OwningCEF_E_WA->GetFormattingArgs(Args, CardIdentity);

	return FText::Format(GetDescription(CardIdentity), Args);
}

FText UGameplayEffect_CardExecution::GetDescription(const FCardIdentity& CardIdentity)
{
	return UCardShooterUtilityLibrary::FormatTextForDisplay(EffectDesc);
}
