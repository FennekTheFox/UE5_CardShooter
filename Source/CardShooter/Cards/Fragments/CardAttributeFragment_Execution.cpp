#include "CardAttributeFragment_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actors/CardShooterCharacter.h"
#include "Gameplay/GameplayAbility_CardExecution.h"
#include "Gameplay/GameplayEffect_CardExecution.h"
#include "Cards/Augments/CardAugment.h"

#define LOCTEXT_NAMESPACE "CardEffectFragment"

void UCardExecutionInstance::StartExecutionInstance(APawn* InExecutingPawn, const FCardExecution_AdditionalData& InAdditionalData, class UCardEffectFragment_Execution* InExec)
{
	ExecutingPawn = InExecutingPawn;
	AdditionalData = InAdditionalData;
	Exec = InExec;

	ExecutingPawn->GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
		this->ProcessExecutionInstance();
		});
}

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	UCardExecutionInstance* NewInstance = ExecuteCardEffect(ExecutingPawn, AdditionalData);
	RunningInstances.Add(NewInstance);

	NewInstance->OnExecutionCompleted.AddLambda([this, NewInstance](APawn* P, const FCardExecution_AdditionalData& D, UCardEffectFragment_Execution* E)
		{
			RunningInstances.Remove(NewInstance);
		});
}

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Delayed_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData Data, float Delay /*= 1.f*/)
{
	FTimerHandle Handle;
	TWeakObjectPtr<APawn> WeakPawn = ExecutingPawn;
	TWeakObjectPtr<UCardEffectFragment_Execution> WeakThis = this;

	ExecutingPawn->GetWorldTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([WeakThis, WeakPawn, Data]()
			{
				if (WeakPawn.IsValid() || WeakThis.IsValid())
				{
					WeakThis->ExecuteCardEffect(WeakPawn.Get(), Data);
				}
			}),
		Delay, false);
}

UCardExecutionInstance* UCardEffectFragment_Execution::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	if (!ensure(ExecInstanceClass))
	{
		return nullptr;
	};

	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance>(this, ExecInstanceClass);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
}

void UCardEffectFragment_Execution_Compound::FindSubExecutionsOfClass(TSubclassOf<UCardEffectFragment_Execution> Class, TArray<UCardEffectFragment_Execution*>& OutArray)
{
	for (UCardEffectFragment_Execution* SubExec : Executions)
	{
		if (UCardEffectFragment_Execution_Compound* AsCompound = Cast<UCardEffectFragment_Execution_Compound>(SubExec))
		{
			AsCompound->FindSubExecutionsOfClass(Class, OutArray);
		}
		else if (SubExec->IsA(Class))
		{
			OutArray.Add(SubExec);
		}
	}
}

UCardExecutionInstance* UCardEffectFragment_Execution_WithParameters::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance>(this, ExecInstanceClass);

	FCardExecution_AdditionalData PostModificationData = AdditionalData;
	for (const UCardParameter* Param : Parameters)
	{
		if (!Param)
		{
			continue;
		}

		PostModificationData.SetByCallerMetaDataTags.Add(Param->ParameterTag);
		PostModificationData.SetByCallerMetaDataValues.Add(Param->MetaData);
	}

	for (FCardAugmentData AugmentData : AdditionalData.AugmentData)
	{
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =
			AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
		{
			AugmentCDO = AugmentClass->GetDefaultObject<UCardAugment>();
		}

		if (AugmentCDO)
		{
			AugmentCDO->ApplyAugment(PostModificationData, AugmentData);
		}
	}

	NewInstance->StartExecutionInstance(ExecutingPawn, PostModificationData, this);
	return NewInstance;
}

void UCardEffectFragment_Execution_WithParameters::GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity)
{
	//create temporary proxy exec data
	FCardExecution_AdditionalData tempAddData;

	for (const UCardParameter* Param : Parameters)
	{
		if (!Param)
		{
			continue;
		}

		tempAddData.SetByCallerMetaDataTags.Add(Param->ParameterTag);
		tempAddData.SetByCallerMetaDataValues.Add(Param->MetaData);
	}

	//apply the card augments
	for (FCardAugmentData AugmentData : CardIdentity.Augments)
	{
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =
			AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
		{
			AugmentCDO = AugmentClass->GetDefaultObject<UCardAugment>();
		}

		if (AugmentCDO)
		{
			AugmentCDO->ApplyAugment(tempAddData, AugmentData);
		}
	}

	//Iterate over all SetByCaller magnitudes
	for (int32 i = 0; i < tempAddData.SetByCallerMetaDataTags.Num(); i++)
	{
		const FSetByCallerMetaData& MetaData = tempAddData.SetByCallerMetaDataValues[i];

		FGameplayTag Tag = tempAddData.SetByCallerMetaDataTags[i];
		FString TagString = Tag.ToString();

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

		UCardParameter** Param = Parameters.FindByPredicate([Tag](const UCardParameter* A)
			{
				if (!A)
				{
					return false;
				}
				return A->ParameterTag == Tag;
			});

		if (!Param)
		{
			continue;
		}

		Args.Add(
			LastPart + "Magnitude",
			FText::Format(LOCTEXT("CardParameterHoverFormat",
				"<hover id=\"{0}\" relatedExec=\"{1}\" cardidentityhandle=\"{2}\" style=\"HoverContent_Param\">{3}</>"),
				FText::FromString((*Param)->GetPathName()),
				FText::FromString(this->GetPathName()),
				FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardIdentityHandle(CardIdentity).Index),
				MetaData.GetDisplayText())
		);

		Args.Add(
			LastPart + "Parameter",
			MetaData.ParameterName
		);
	}
}

#undef LOCTEXT_NAMESPACE



