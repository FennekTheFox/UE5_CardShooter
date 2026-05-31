#include "CardEffectFragment_EffectToSelf.h"

#include "Gameplay/GameplayEffect_CardExecution.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../Util/CardShooterUtility.h"


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

	for (int32 i = 0; i < AdditionalData.SetByCallerMetaDataTags.Num(); i++)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(AdditionalData.SetByCallerMetaDataTags[i],
			AdditionalData.SetByCallerMetaDataValues[i].Magnitude);
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

FText UCardEffectFragment_ApplyEffectToOwner::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	FFormatNamedArguments Args;
	GetFormattingArgs(Args, CardIdentity);
	return FText::Format(GetRawLocalizableDescription(CardIdentity), Args);
}

UCardEffectFragment_ApplyEffectToOwner::UCardEffectFragment_ApplyEffectToOwner()
{
	ExecInstanceClass = UCardExecutionInstance_ApplyEffectToOwner::StaticClass();
}

FText UCardEffectFragment_ApplyEffectToOwner::GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const
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

		return UCardShooterUtilityLibrary::FormatTextForDisplay(
			FText::Format(LOCTEXT("EffectToSelfDesc", "{0} <hover id=\"{1}\" relatedExec=\"{2}\" cardidentityhandle=\"{3}\" style=\"HoverContent\">{4}</>."),
				Base,
				FText::FromString(AsGE_CE->GetPathName()),
				FText::FromString(this->GetPathName()),
				FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardIdentityHandle(CardIdentity).Index),
				AsGE_CE->GetSimpleLocalizableName())
		);
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE