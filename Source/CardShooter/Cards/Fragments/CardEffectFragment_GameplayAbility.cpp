#include "CardEffectFragment_GameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GameplayAbility_CardExecution.h"


#define LOCTEXT_NAMESPACE "CardEffects"


void UCardEffectFragment_GameplayAbility::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	Super::ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		return;
	}

	FGameplayAbilitySpec AbilitySpec(AbilityToExecute.Ability);
	FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);

	//This relies on the ability 100% getting executed, else it wont get removed properly
	TSharedPtr<FDelegateHandle> OnEndedHandle = MakeShared<FDelegateHandle>();
	TSharedPtr<FDelegateHandle> OnFailedHandle = MakeShared<FDelegateHandle>();
	*OnEndedHandle = ASC->OnAbilityEnded.AddLambda([ASC, SpecHandle, AbilitySpec, OnEndedHandle, OnFailedHandle](const FAbilityEndedData& Data) {
		if (Data.AbilityThatEnded == AbilitySpec.Ability)
		{
			ASC->ClearAbility(SpecHandle);

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

	*OnFailedHandle = ASC->AbilityFailedCallbacks.AddLambda([ASC, SpecHandle, AbilitySpec, OnEndedHandle, OnFailedHandle](const UGameplayAbility* Ability, const FGameplayTagContainer&) {
		if (Ability == AbilitySpec.Ability)
		{
			ASC->ClearAbility(SpecHandle);

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
			[ASC, SpecHandle, AdditionalData]
			()
			{

				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(SpecHandle);
				if (Spec)
				{
					if (!ensure(Spec->GetAbilityInstances().IsEmpty()))
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

FText UCardEffectFragment_GameplayAbility::GetLocalizableDescription_Implementation()
{
	if (!ensure(AbilityToExecute.Ability))
	{
		return FText::GetEmpty();
	}

	if (UGameplayAbility_CardExecution* AsGA_CE = Cast<UGameplayAbility_CardExecution>(AbilityToExecute.Ability->GetDefaultObject()))
	{
		return FText::Format(FText::FromString("{0}: {1}"), AsGA_CE->GetName(), AsGA_CE->GetDescription());
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE