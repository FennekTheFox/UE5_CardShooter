#include "GameplayEffect_CardExecution.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"
#include "Util/CardShooterUtility.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"

UGameplayEffect_CardExecution::UGameplayEffect_CardExecution()
{

}

FText UGameplayEffect_CardExecution::GetDescription(const FCardIdentity& CardIdentity, const UCardEffectFragment_Execution_WithParameters* OwningCEF_E_WA) const
{
	FFormatNamedArguments Args;
	OwningCEF_E_WA->GetFormattingArgs(Args, CardIdentity);

	return FText::Format(GetDescription(CardIdentity), Args);
}

FText UGameplayEffect_CardExecution::GetDescription(const FCardIdentity& CardIdentity) const
{
	return UCardShooterUtilityLibrary::FormatTextForDisplay(EffectDesc);
}

void UGameplayEffect_CardExecution::GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const FCardIdentity& InCardIdentity, const UCardEffectFragment_Execution_WithParameters* RelatedExec /*= nullptr*/) const
{
	OutText = GetDescription(InCardIdentity, RelatedExec);
}

void UGameplayEffect_CardExecution::GetFormattingArgs(const UCardEffectFragment_Execution_WithParameters* RelatedExec, FFormatNamedArguments& FormattingArguments) const
{
	UCardEffectFragment_Execution_WithParameters::AppendFormattingArgsFromHoverContextObjects(RelatedExec, HoverContextObjects, FormattingArguments);
}

void UGameplayEffect_CardExecution::GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec) const
{
	OutText = FText::Format(NSLOCTEXT("CardAttributes", "GameplayEffectHowerTitle", "Effect: {0}"), GetSimpleLocalizableName());
}
