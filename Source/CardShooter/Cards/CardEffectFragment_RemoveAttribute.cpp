#include "CardEffectFragment_RemoveAttribute.h"

#include "Actors/CardShooterCharacter.h"

#define LOCTEXT_NAMESPACE "CardEffects"

void UCardEffectFragment_Execution_RemoveAttributeFromNextCards::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	//TODO This may not be player exclusive, but probably will. If not this needs to change
	if (ACardShooterCharacter* AsCSPCharacter = Cast<ACardShooterCharacter>(ExecutingPawn))
	{
		int32& ExistingCount = AsCSPCharacter->AttributesToRemoveFromExecutions.FindOrAdd(AttributeClass);
		ExistingCount += Count;
	}
}

FText UCardEffectFragment_Execution_RemoveAttributeFromNextCards::GetLocalizableDescription_Implementation()
{
	if (!ensure(AttributeClass))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("RemoveAttributesToNextDesc", "Removes <hover id=\"{0}\">{1}</> from the next {2} cards."),
		FText::FromString(AttributeClass->GetDefaultObject()->GetPathName()),
		Cast<UCardEffectFragment_Attribute>(AttributeClass->GetDefaultObject())->GetLocalizableName(),
		Count);
}

#undef LOCTEXT_NAMESPACE