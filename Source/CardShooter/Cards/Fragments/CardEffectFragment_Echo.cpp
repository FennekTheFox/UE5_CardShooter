#include "CardEffectFragment_Echo.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Echo::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(LOCTEXT("EchoAttrName", "Echo({0}x | {1}s)"), EchoCount, EchoDelay);
}

FText UCardEffectFragment_Echo::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return FText::Format(LOCTEXT("EchoAttrDesc", "Card effect repeats {0} times every {1} seconds."), EchoCount, EchoDelay);
}

UCardEffectFragment_Echo::UCardEffectFragment_Echo()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Echo", "Echo");
}

#undef LOCTEXT_NAMESPACE