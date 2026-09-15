#include "CardEffectFragment_GameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GameplayAbility_CardExecution.h"
#include "Cards/Augments/CardAugment.h"
#include "Gameplay/GameplayEffect_CardExecution.h"
#include "Util/CardShooterUtility.h"
#include "UI/RichTextHoverDecorator.h"


#define LOCTEXT_NAMESPACE "CardEffects"

namespace CardGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Event_InitializeCardAbility, "Gameplay.Event.InitializeCardAbility");
}

void UCardExecutionInstance_GameplayAbility::ProcessExecutionInstance()
{
	UCardEffectFragment_GameplayAbility* AsGA = Cast<UCardEffectFragment_GameplayAbility>(Exec);

	if (!ensure(AsGA))
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

	FGameplayAbilitySpec AbilitySpec(AsGA->AbilityToExecute.Ability, 1, -1, this);
	FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);

	//This relies on the ability 100% getting executed, else it wont get removed properly
	TSharedPtr<FDelegateHandle> OnEndedHandle = MakeShared<FDelegateHandle>();
	TSharedPtr<FDelegateHandle> OnFailedHandle = MakeShared<FDelegateHandle>();
	*OnEndedHandle = ASC->OnAbilityEnded.AddLambda([this, ASC, SpecHandle, AbilitySpec, OnEndedHandle, OnFailedHandle](const FAbilityEndedData& Data) {
		if (Data.AbilitySpecHandle == SpecHandle)
		{
			ASC->ClearAbility(SpecHandle);
			OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);

			if (OnEndedHandle->IsValid())
			{
				ASC->OnAbilityEnded.Remove(*OnEndedHandle);
			}
			if (OnFailedHandle->IsValid())
			{
				ASC->AbilityFailedCallbacks.Remove(*OnFailedHandle);
			}
		}
		}
	);

	*OnFailedHandle = ASC->AbilityFailedCallbacks.AddLambda([this, ASC, SpecHandle, AbilitySpec, OnEndedHandle, OnFailedHandle](const UGameplayAbility* Ability, const FGameplayTagContainer&) {
		if (Ability == AbilitySpec.Ability)
		{
			ASC->ClearAbility(SpecHandle);
			OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);

			if (OnEndedHandle->IsValid())
			{
				ASC->OnAbilityEnded.Remove(*OnEndedHandle);
			}
			if (OnFailedHandle->IsValid())
			{
				ASC->AbilityFailedCallbacks.Remove(*OnFailedHandle);
			}
		}
		}
	);

	ExecutingPawn->GetWorld()->GetTimerManager()
		.SetTimerForNextTick(FTimerDelegate::CreateLambda(
			[this, ASC, SpecHandle]
			()
			{

				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(SpecHandle);
				if (Spec)
				{
					//TODO: this triggers on certain abilities, which execute on server and client
					//not fatal, only annoying and not intended
					if (!/*ensure*/(Spec->GetAbilityInstances().IsEmpty()))
					{
						UGameplayAbility_CardExecution* GA_CE = Cast<UGameplayAbility_CardExecution>(Spec->GetAbilityInstances().Last());
						if (GA_CE)
						{
							GA_CE->AdditionalData = AdditionalData;
							GA_CE->PostAdditionalDataSet();
						}
					}
				}
			}));
}

FText UCardEffectFragment_GameplayAbility::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	FFormatNamedArguments Args;
	GetFormattingArgs(Args, CardIdentity);

	return	FText::Format(UCardShooterUtilityLibrary::FormatTextForDisplay(GetRawLocalizableDescription(CardIdentity)), Args);
}

UCardEffectFragment_GameplayAbility::UCardEffectFragment_GameplayAbility()
{
	ExecInstanceClass = UCardExecutionInstance_GameplayAbility::StaticClass();
}

FText UCardEffectFragment_GameplayAbility::GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const
{
	if (!ensure(AbilityToExecute.Ability))
	{
		return FText::GetEmpty();
	}

	if (UGameplayAbility_CardExecution* AsGA_CE = Cast<UGameplayAbility_CardExecution>(AbilityToExecute.Ability->GetDefaultObject()))
	{
		if (AsGA_CE->GetName().IsEmpty())
		{
			return AsGA_CE->GetDescription(CardIdentity);
		}
		return FText::Format(FText::FromString("{0}: {1}"),
			AsGA_CE->GetName(),
			AsGA_CE->GetDescription(CardIdentity));
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE
