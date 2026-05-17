#include "CardEffectFragment_Finisher.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Finisher::GetLocalizableName_Implementation()
{
	return LOCTEXT("FinisherAttrName", "Finisher");
}

FText UCardEffectFragment_Finisher::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("FinisherAttrDesc", "This card always shuffles to the back of your deck.");
}

#undef LOCTEXT_NAMESPACE