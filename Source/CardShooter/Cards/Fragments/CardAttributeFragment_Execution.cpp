#include "CardAttributeFragment_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actors/CardShooterCharacter.h"
#include "Gameplay/GameplayAbility_CardExecution.h"
#include "Gameplay/GameplayEffect_CardExecution.h"
#include "Cards/Augments/CardAugment.h"
#include "Cards/CardData.h"

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

UCardExecutionInstance* UCardEffectFragment_Execution::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData, UCardEffectFragment_Execution* ParentExecution /*= nullptr*/)
{
	if (!ensure(ExecInstanceClass))
	{
		return nullptr;
	};

	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance>(this, ExecInstanceClass);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
}

bool UCardEffectFragment_Execution::CanActivateExecution(class ACharacter* Avatar, const FCardIdentity& CardIdentity)
{
	//This base class doesnt really get used anymore, and is especially useless for the conditions...
	ensure(false);
	return true;
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

void UCardEffectFragment_Execution_Compound::PostLoad()
{
	Super::PostLoad();

	for (UCardEffectFragment_Execution* Execution : Executions)
	{
		if (Execution)
		{
			Execution->ParentCompound = this;
		}
	}
}

void UCardEffectFragment_Execution_WithParameters::GatherParameters(FCardExecution_AdditionalData& OutData) const
{
	for (const UCardParameter* Param : Parameters)
	{
		if (!Param)
		{
			continue;
		}

		OutData.SetByCallerMetaDataTags.Add(Param->ParameterTag);
		OutData.SetByCallerMetaDataValues.Add(Param->MetaData);
	}
}

void UCardEffectFragment_Execution_WithParameters::AppendFormattingArgsFromHoverContextObjects(const UCardEffectFragment_Execution_WithParameters* RelatedExec, const TMap<FString, TSubclassOf<class UObject>>& HoverContextObjects, FFormatNamedArguments& Args, FCardIdentity CardIdentity)
{
	if (!ensure(RelatedExec))
	{
		return;
	}

	for (auto&& KVPair : HoverContextObjects)
	{
		if (!ensure(KVPair.Value))
		{
			continue;
		}

		FText DisplayText = FText::GetEmpty();

		if (UGameplayEffect_CardExecution* AsGE_CE = Cast<UGameplayEffect_CardExecution>(KVPair.Value->GetDefaultObject()))
		{
			DisplayText = AsGE_CE->GetSimpleLocalizableName();
		}
		else
			if (KVPair.Value->GetDefaultObject()->Implements<UHoverableContext>())
			{
				IHoverableContext::Execute_GetHoverText(
					KVPair.Value->GetDefaultObject(),
					DisplayText
				);
			}

		FText Style = FText::FromString("HoverContent");
		if (IHoverableContext* AsHoverableContext = Cast<IHoverableContext>(KVPair.Value->GetDefaultObject()))
		{
			AsHoverableContext->GetHoverStyle(Style);
		}

		Args.Add(KVPair.Key, FText::Format(LOCTEXT("AbilityReferencedEffectFormat",
			"<hover id=\"{0}\" relatedExec=\"{1}\" cardidentityhandle=\"{2}\" style=\"{3}\">{4}</>"),
			FText::FromString(KVPair.Value->GetDefaultObject()->GetPathName()),
			FText::FromString(RelatedExec->GetPathName()),
			FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardCachedIdentityHandle(CardIdentity).Index),
			Style,
			DisplayText
		));
	}
}

UCardExecutionInstance* UCardEffectFragment_Execution_WithParameters::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData, UCardEffectFragment_Execution* ParentExecution /*= nullptr*/)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance>(this, ExecInstanceClass);

	FCardExecution_AdditionalData PostModificationData = AdditionalData;

	GatherParameters(PostModificationData);

	for (FCardAugmentData AugmentData : AdditionalData.AugmentData)
	{
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =	AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
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

bool UCardEffectFragment_Execution_WithParameters::CanActivateExecution(ACharacter* Avatar, const FCardIdentity& CardIdentity)
{
	if (!ensure(Avatar))
	{
		return false;
	}

	FCardExecution_AdditionalData TmpData;
	GatherParameters(TmpData);

	TArray<UItem_CardAugment*> Augments;
	UCardData::GetAugments(CardIdentity, Augments);
	for (UItem_CardAugment* AugmentItem : Augments)
	{
		if (!AugmentItem)
		{
			continue;
		}
		FCardAugmentData AugmentData = AugmentItem->AugmentData;
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =
			AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
		{
			AugmentCDO = AugmentClass->GetDefaultObject<UCardAugment>();
		}

		if (AugmentCDO)
		{
			AugmentCDO->ApplyAugment(TmpData, AugmentData);
		}
	}

	for (UCardActivationCondition* ActivationCondition : ActivationConditions)
	{
		if (!ActivationCondition->CheckCondition(Avatar, TmpData))
		{
			return false;
		}
	}

	return true;
}

void UCardEffectFragment_Execution_WithParameters::GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity) const
{
	if (UCardEffectFragment_Execution_WithParameters* ParentWithParams = Cast<UCardEffectFragment_Execution_WithParameters>(ParentCompound))
	{
		ParentWithParams->GetFormattingArgs(Args, CardIdentity);
	}

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
	TArray<UItem_CardAugment*> Augments;
	UCardData::GetAugments(CardIdentity, Augments);
	for (UItem_CardAugment* AugmentItem : Augments)
	{
		if (!AugmentItem)
		{
			continue;
		}
		FCardAugmentData AugmentData = AugmentItem->AugmentData;
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
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

		UCardParameter* const* Param = Parameters.FindByPredicate([Tag](const UCardParameter* A)
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

		FText FormatingBase = FText::GetEmpty();
		IHoverableContext::Execute_GetHoverText(*Param, FormatingBase);

		Args.Add(
			LastPart + "Magnitude",
			FText::Format(FormatingBase,
				FText::FromString((*Param)->GetPathName()),
				FText::FromString(this->GetPathName()),
				FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardCachedIdentityHandle(CardIdentity).Index),
				(*Param)->GetStyle(MetaData),
				MetaData.GetDisplayText())
		);

		Args.Add(
			LastPart + "Parameter",
			MetaData.ParameterName
		);
	}

	UCardEffectFragment_Execution_WithParameters::AppendFormattingArgsFromHoverContextObjects(this, HoverContextObjects, Args, CardIdentity);
}

bool UCardActivationCondition::CheckCondition_Implementation(ACharacter* Avatar, const FCardExecution_AdditionalData& ExecutionData)
{
	ensure(false);
	return false;
}

#undef LOCTEXT_NAMESPACE
