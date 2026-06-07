#include "CardEffectFragment_Utility.h"
#include "Cards/CardData.h"
#include "Util/CardShooterUtility.h"

#define LOCTEXT_NAMESPACE "CardEffects"

UE_DEFINE_GAMEPLAY_TAG(TAG_UtilityPrimaryCooldown, "Gameplay.Effect.Parameters.PrimaryUtilityCooldown");
UE_DEFINE_GAMEPLAY_TAG(TAG_UtilitySecondaryCooldown, "Gameplay.Effect.Parameters.SecondaryUtilityCooldown");

UCardEffectFragment_Utility::UCardEffectFragment_Utility()
{
	static ConstructorHelpers::FClassFinder<UCardParameter> BPCardParamClassFinder(
		TEXT("/Game/Blueprint/Misc/CardParameters/SGParams/BP_CardParameter_SG_Base"));


	if (ensure(BPCardParamClassFinder.Succeeded()))
	{
		CooldownParamClass = BPCardParamClassFinder.Class;
	}
}

void UCardEffectFragment_Utility::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	if (!Parameters.IsEmpty())
	{
		return;
	}

	if (!CooldownParamClass)
	{
		return;
	}

	FSetByCallerMetaData DefaultCooldowns;
	DefaultCooldowns.Magnitude = 10.f;
	DefaultCooldowns.ScalingGroup = FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Effect.ScalingGroups.Cooldown"));
	DefaultCooldowns.DisplayType = ESetByCallerMagnitudeDisplayType::Flat;
	DefaultCooldowns.MagnitudeRange = FFloatRange(0.1f, BIG_NUMBER);

	UCardParameter* PrimaryCooldownParam = NewObject<UCardParameter>(this, CooldownParamClass, TEXT("PrimaryUtilityCooldownParam"));
	PrimaryCooldownParam->ParameterTag = TAG_UtilityPrimaryCooldown;
	PrimaryCooldownParam->MetaData = DefaultCooldowns;

	UCardParameter* SecondaryCooldownParam = NewObject<UCardParameter>(this, CooldownParamClass, TEXT("SecondaryUtilityCooldownParam"));
	SecondaryCooldownParam->ParameterTag = TAG_UtilitySecondaryCooldown;
	SecondaryCooldownParam->MetaData = DefaultCooldowns;


	Parameters.Add(PrimaryCooldownParam);
	Parameters.Add(SecondaryCooldownParam);
}

FText UCardEffectFragment_Utility::GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const
{
	UCardData* OwningCard = Cast<UCardData>(GetOuter());

	float PrimaryUtilityCooldown = -1.f;
	GetParameterValue(TAG_UtilityPrimaryCooldown, PrimaryUtilityCooldown, CardIdentity);

	float SecondaryUtilityCooldown = -1.f;
	GetParameterValue(TAG_UtilitySecondaryCooldown, SecondaryUtilityCooldown, CardIdentity);

	if (OwningCard)
	{
		if (!OwningCard->SecondaryExecution || PrimaryUtilityCooldown == SecondaryUtilityCooldown)
		{
			return UCardShooterUtilityLibrary::FormatTextForDisplay(LOCTEXT("UtilityAttrDesc", "This card can be used in a decks utility slot. If used in a utility slot, this card has a cooldown of {PrimaryUtilityCooldownMagnitude}s."));
		}
		return UCardShooterUtilityLibrary::FormatTextForDisplay(LOCTEXT("UtilityAttrDesc", "This card can be used in a decks utility slot. If used in a utility slot, this card has a cooldown of {PrimaryUtilityCooldownMagnitude}s for its primary execution, or {SecondaryUtilityCooldownMagnitude}s for its secondary execution."));
	}


	return FText::GetEmpty();
}

FText UCardEffectFragment_Utility::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	UCardData* OwningCard = Cast<UCardData>(GetOuter());

	float PrimaryUtilityCooldown = -1.f;
	GetParameterValue(TAG_UtilityPrimaryCooldown, PrimaryUtilityCooldown, CardIdentity);

	float SecondaryUtilityCooldown = -1.f;
	GetParameterValue(TAG_UtilitySecondaryCooldown, SecondaryUtilityCooldown, CardIdentity);

	if (OwningCard || PrimaryUtilityCooldown == SecondaryUtilityCooldown)
	{
		if (!OwningCard->SecondaryExecution)
		{
			return FText::Format(LOCTEXT("UtilityAttrName", "Utility({0}s)"), PrimaryUtilityCooldown);
		}

		return FText::Format(LOCTEXT("UtilityAttrName", "Utility({0}s|{1}s)"), PrimaryUtilityCooldown, SecondaryUtilityCooldown);
	}


	return FText::GetEmpty();
}

FText UCardEffectFragment_Utility::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{

	FFormatNamedArguments Args;
	GetFormattingArgs(Args, CardIdentity);

	return FText::Format(GetRawLocalizableDescription(CardIdentity), Args);
}
#undef LOCTEXT_NAMESPACE