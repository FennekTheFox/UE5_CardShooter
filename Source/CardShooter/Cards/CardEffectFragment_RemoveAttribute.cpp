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

FText UCardEffectFragment_Execution_RemoveAttributeFromNextCards::GetName_Implementation()
{
	if (!ensure(AttributeClass))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("RemoveAttributesToNextName", "Removes {0} from the next {1} cards."), Cast<UCardEffectFragment_ExecutionAttribute>(AttributeClass->GetDefaultObject())->GetDescription(), Count);
}

#undef LOCTEXT_NAMESPACE