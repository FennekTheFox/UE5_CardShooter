#include "CardEffectFragment_EffectToSelf.h"

#include "Gameplay/CardExecutionEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


#define LOCTEXT_NAMESPACE "CardEffects"

void UCardEffectFragment_Execution_ApplyEffectToOwner::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
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

	FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FText UCardEffectFragment_Execution_ApplyEffectToOwner::GetName_Implementation()
{
	if (!ensure(GameplayEffect))
	{
		return FText::GetEmpty();
	}

	if (UGameplayEffect_CardExecution* AsGA_CE = Cast<UGameplayEffect_CardExecution>(GameplayEffect->GetDefaultObject()))
	{
		return AsGA_CE->GetDescription();
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE