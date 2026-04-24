#include "CardData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectAbilitySystemComponent.h"
#include "Templates/SharedPointer.h"

FText UCardEffectFragment_Base::GetFragmentLabel()
{
	return FText::FromString("Unassigned Label");
}

FText UCardEffectFragment_Root::GetFragmentDescription()
{
	return FText::FromString("Unassigned Description");
}

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn)
{

}

void UCardEffectFragment_Execution_GameplayAbility::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn)
{
	Super::Server_ExecuteCardEffect_Implementation(ExecutingPawn);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		return;
	}

	FGameplayAbilitySpec AbilitySpec(AbilityToExecute.Ability);
	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);


	//NOTE: This does not work because giving the ability may be delayed
	//if (ASC->TryActivateAbility(Handle))
	//{
	//	ASC->OnAbilityEnded.AddLambda([ASC, Handle, AbilitySpec](const FAbilityEndedData& Data) {
	//		if (Data.AbilityThatEnded == AbilitySpec.Ability)
	//		{
	//			ASC->ClearAbility(Handle);
	//		}
	//		}
	//	);
	//}

	//ASC->ClearAbility(Handle);

	//This relies on the ability 100% getting executed, else it wont get removed properly
	TSharedPtr<FDelegateHandle> OnEndedHandle = MakeShared<FDelegateHandle>();
	TSharedPtr<FDelegateHandle> OnFailedHandle = MakeShared<FDelegateHandle>();
	*OnEndedHandle = ASC->OnAbilityEnded.AddLambda([ASC, Handle, AbilitySpec, OnEndedHandle, OnFailedHandle](const FAbilityEndedData& Data) {
		if (Data.AbilityThatEnded == AbilitySpec.Ability)
		{
			ASC->ClearAbility(Handle);

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

	*OnFailedHandle = ASC->AbilityFailedCallbacks.AddLambda([ASC, Handle, AbilitySpec, OnEndedHandle, OnFailedHandle](const UGameplayAbility* Ability, const FGameplayTagContainer&) {
		if (Ability == AbilitySpec.Ability)
		{
			ASC->ClearAbility(Handle);

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
}

 void UCardData::GetIdentity(FCardIdentity& Identity)
{
	Identity.AssetTag = AssetTag;
	Identity.CardDataPath = FSoftObjectPath(this);
}
