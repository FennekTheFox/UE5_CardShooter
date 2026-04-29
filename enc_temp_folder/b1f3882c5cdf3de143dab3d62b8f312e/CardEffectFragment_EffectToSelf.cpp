#include "CardEffectFragment_EffectToSelf.h"

#include "Gameplay/CardExecutionEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


#define LOCTEXT_NAMESPACE "CardEffects"

void UCardEffectFragment_ApplyEffectToOwner::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	if (!ensure(ExecutingPawn && GameplayEffect))
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, GameplayEffectLevel, ASC->MakeEffectContext());

	for (auto&& KVPair : SetByCallerMagnitudes)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(KVPair.Key, KVPair.Value.Magnitude);
	}

	FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FText UCardEffectFragment_ApplyEffectToOwner::GetLocalizableDescription_Implementation()
{
	if (!ensure(GameplayEffect))
	{
		return FText::GetEmpty();
	}

	if (UGameplayEffect_CardExecution* AsGE_CE = Cast<UGameplayEffect_CardExecution>(GameplayEffect->GetDefaultObject()))
	{
		FFormatNamedArguments Args;
		GetSetByCallerFormattingArgs(Args);

		return FText::Format(LOCTEXT("EffectToSelfDesc", "Grants <hover id=\"{0}\" effect=\"{1}\">{2}</>."),
			FText::FromString(AsGE_CE->GetPathName()),
			FText::FromString(this->GetPathName()),
			FText::Format(AsGE_CE->GetDetailedLocalizableName(), Args));
	}

	ensure(false);
	return FText::GetEmpty();
}

void UCardEffectFragment_ApplyEffectToOwner::GetSetByCallerFormattingArgs(FFormatNamedArguments& Args)
{
	for (auto&& KVPair : SetByCallerMagnitudes)
	{
		const FSetByCallerMetaData& MetaData = KVPair.Value;

		FString TagString = KVPair.Key.ToString();

		FString Left;
		FString Right;

		FString LastPart = TagString;

		if (TagString.Split(
			TEXT("."),
			&Left,
			&Right,
			ESearchCase::IgnoreCase,
			ESearchDir::FromEnd))
		{
			LastPart = Right;
		}

		Args.Add(
			LastPart + "Magnitude",
			MetaData.GetDisplayText()
		);

		Args.Add(
			LastPart + "Parameter",
			MetaData.ParameterName
		);
	}
}

#undef LOCTEXT_NAMESPACE

FText FSetByCallerMetaData::GetDisplayText() const 
{
	switch (DisplayType)
	{
	case ESetByCallerMagnitudeDisplayType::Flat: 
		return FText::AsNumber(Magnitude);
	case ESetByCallerMagnitudeDisplayType::Percent: 
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(100.f * Magnitude));
	case ESetByCallerMagnitudeDisplayType::PercentOneMinus: 
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(100.f * (1.f - Magnitude)));
	}

	ensure(false);
	return FText::GetEmpty();
}
