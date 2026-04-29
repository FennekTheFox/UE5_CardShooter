#include "CardEffectFragment_Echo.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Echo::GetLocalizableName_Implementation()
{
	return FText::Format(LOCTEXT("EchoAttrName", "Echo({0}x | {1}s)"), EchoCount, EchoDelay);
}

FText UCardEffectFragment_Echo::GetLocalizableDescription_Implementation()
{
	return FText::Format(LOCTEXT("EchoAttrDesc", "Card effect repeats {0} times every {1} seconds."), EchoCount, EchoDelay);
}

#undef LOCTEXT_NAMESPACE