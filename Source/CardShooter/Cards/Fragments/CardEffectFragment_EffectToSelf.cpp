#include "CardEffectFragment_EffectToSelf.h"

#include "Gameplay/GameplayEffect_CardExecution.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


#define LOCTEXT_NAMESPACE "CardEffects"


void UCardExecutionInstance_ApplyEffectToOwner::ProcessExecutionInstance()
{
	UCardEffectFragment_ApplyEffectToOwner* AsEffectToOwner = Cast<UCardEffectFragment_ApplyEffectToOwner>(Exec);

	if (!ensure(AsEffectToOwner))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	if (!ensure(ExecutingPawn && AsEffectToOwner->GameplayEffect))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(AsEffectToOwner->GameplayEffect, AsEffectToOwner->GameplayEffectLevel, Context);

	for (auto&& KVPair : AsEffectToOwner->SetByCallerMagnitudes)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(KVPair.Key, KVPair.Value.Magnitude);
	}

	FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	

	if (AsEffectToOwner->bResolveImmediately)
	{
		//If the execution is to resolve immediately
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
	}
	else
	{
		//If not, then we wait for the effect to expire
		if (EffectHandle.IsValid())
		{
			ASC->OnGameplayEffectRemoved_InfoDelegate(EffectHandle)
				->AddLambda([this](const FGameplayEffectRemovalInfo& RemovalInfo)
					{
						OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
					});
		}
	}
}

UCardExecutionInstance* UCardEffectFragment_ApplyEffectToOwner::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance_ApplyEffectToOwner>(this);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
}

FText UCardEffectFragment_ApplyEffectToOwner::GetLocalizableDescription_Implementation()
{
	if (!ensure(GameplayEffect))
	{
		return FText::GetEmpty();
	}

	if (UGameplayEffect_CardExecution* AsGE_CE = Cast<UGameplayEffect_CardExecution>(GameplayEffect->GetDefaultObject()))
	{
		FText Base = FText::GetEmpty();

		switch (EffectInclination)
		{
		case EEffectInclination::Benefitial: Base = LOCTEXT("EffectToSelfDescBaseBenefitial", "Grants"); break;
		case EEffectInclination::Detrimental: Base = LOCTEXT("EffectToSelfDescBaseDetrimental", "Inflicts"); break;
		}

		FFormatNamedArguments Args;
		GetSetByCallerFormattingArgs(Args);

		return FText::Format(LOCTEXT("EffectToSelfDesc", "{0} <hover id=\"{1}\" relatedEffectExec=\"{2}\">{3}</>."),
			Base,
			FText::FromString(AsGE_CE->GetPathName()),
			FText::FromString(this->GetPathName()),
			FText::Format(AsGE_CE->GetDetailedLocalizableName(), Args));
	}

	ensure(false);
	return FText::GetEmpty();
}
#undef LOCTEXT_NAMESPACE