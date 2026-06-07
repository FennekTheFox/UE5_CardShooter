#include "CardEffectFragment_AddAttribute.h"
#include "Actors/CardShooterCharacter.h"

#define LOCTEXT_NAMESPACE "CardEffects"

void UCardExecutionInstance_AddAttributeToNextCards::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_AddAttributeToNextCards* AsAddAttribute = Cast<UCardEffectFragment_Execution_AddAttributeToNextCards>(Exec);

	if (!ensure(AsAddAttribute))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	//TODO This may not be player exclusive, but probably will. If not this needs to change
	if (ACardShooterCharacter* AsCSPCharacter = Cast<ACardShooterCharacter>(ExecutingPawn))
	{
		int32& ExistingCount = AsCSPCharacter->AttributesToAddToExecutions.FindOrAdd(AsAddAttribute->Attribute);
		ExistingCount += AsAddAttribute->Count;
	}

	OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
}

FText UCardEffectFragment_Execution_AddAttributeToNextCards::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	if (!ensure(Attribute))
	{
		return FText::GetEmpty();
	}

	return FText::Format(LOCTEXT("AddAttributesToNextDesc", "Adds <hover id=\"{0}\" style=\"HoverContent\">{1}< / > to the next{2} cards."),
		FText::FromString(Attribute->GetPathName()),
		Attribute->GetLocalizableName(CardIdentity),
		Count);
}

UCardEffectFragment_Execution_AddAttributeToNextCards::UCardEffectFragment_Execution_AddAttributeToNextCards()
{
	ExecInstanceClass = UCardExecutionInstance_AddAttributeToNextCards::StaticClass();
}

#undef LOCTEXT_NAMESPACE

