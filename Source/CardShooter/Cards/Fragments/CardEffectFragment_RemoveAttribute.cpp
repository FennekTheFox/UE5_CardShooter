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


FText UCardEffectFragment_Execution_RemoveAttributeFromNextCards::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	if (!ensure(AttributeClass))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("RemoveAttributesToNextDesc", "Removes <hover id=\"{0}\" cardidentityhandle=\"{1}\">{2}</> from the next {3} cards."),
		FText::FromString(AttributeClass->GetDefaultObject()->GetPathName()),
		FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardIdentityHandle(CardIdentity).Index),
		Cast<UCardEffectFragment_Attribute>(AttributeClass->GetDefaultObject())->GetLocalizableName(),
		Count);
}

UCardEffectFragment_Execution_RemoveAttributeFromNextCards::UCardEffectFragment_Execution_RemoveAttributeFromNextCards()
{
	ExecInstanceClass = UCardExecutionInstance_RemoveAttributeFromNextCards::StaticClass();
}

#undef LOCTEXT_NAMESPACE