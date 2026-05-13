#include "CardEffectFragment_RemoveAttribute.h"

#include "Actors/CardShooterCharacter.h"

#define LOCTEXT_NAMESPACE "CardEffects"


void UCardExecutionInstance_RemoveAttributeFromNextCards::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_RemoveAttributeFromNextCards* AsRemoveattribute = Cast<UCardEffectFragment_Execution_RemoveAttributeFromNextCards>(Exec);

	if (!ensure(AsRemoveattribute))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	//TODO This may not be player exclusive, but probably will. If not this needs to change
	if (ACardShooterCharacter* AsCSPCharacter = Cast<ACardShooterCharacter>(ExecutingPawn))
	{
		int32& ExistingCount = AsCSPCharacter->AttributesToRemoveFromExecutions.FindOrAdd(AsRemoveattribute->AttributeClass);
		ExistingCount += AsRemoveattribute->Count;
	}

	OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
}

UCardExecutionInstance* UCardEffectFragment_Execution_RemoveAttributeFromNextCards::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance_RemoveAttributeFromNextCards>(this);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
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