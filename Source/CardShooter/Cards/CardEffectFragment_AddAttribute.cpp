#include "CardEffectFragment_AddAttribute.h"
#include "Actors/CardShooterCharacter.h"

#define LOCTEXT_NAMESPACE "CardEffects"

void UCardEffectFragment_Execution_AddAttributeToNextCards::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	//TODO This may not be player exclusive, but probably will. If not this needs to change
	if (ACardShooterCharacter* AsCSPCharacter = Cast<ACardShooterCharacter>(ExecutingPawn))
	{
		int32& ExistingCount = AsCSPCharacter->AttributesToAddToExecutions.FindOrAdd(Attribute);
		ExistingCount += Count;
	}
}

FText UCardEffectFragment_Execution_AddAttributeToNextCards::GetName_Implementation()
{
	if (!ensure(Attribute))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("AddAttributesToNextName", "Adds {0} to the next {1} cards."), Attribute->GetDescription(), Count);
}

#undef LOCTEXT_NAMESPACE