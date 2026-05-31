#include "CardEffectFragment_Combo.h"


#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Combo::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(LOCTEXT("ComboAttrDesc", "Combo({0}x|{1}s)"),
		FText::AsNumber(ComboCount),
		FText::AsNumber(ComboWindow));
}

FText UCardEffectFragment_Combo::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(LOCTEXT("ComboAttrDesc", "This card can be activated up to {0} times within {1} seconds."),
		FText::AsNumber(ComboCount),
		FText::AsNumber(ComboWindow));
}

#undef LOCTEXT_NAMESPACE