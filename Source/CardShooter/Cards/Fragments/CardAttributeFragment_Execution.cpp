#include "CardAttributeFragment_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actors/CardShooterCharacter.h"
#include "Gameplay/GameplayAbility_CardExecution.h"
#include "Gameplay/GameplayEffect_CardExecution.h"

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
	return nullptr;
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

void UCardEffectFragment_ExecutionWithParameters::GetSetByCallerFormattingArgs(FFormatNamedArguments& Args)
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
	case ESetByCallerMagnitudeDisplayType::Absolute:
		return FText::Format(FText::FromString("{0}"), FText::AsNumber(FMath::Abs(Magnitude)));
	case ESetByCallerMagnitudeDisplayType::AbsolutePercent:
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(FMath::Abs(100.f * Magnitude)));
	case ESetByCallerMagnitudeDisplayType::AbsolutePercentOneOver:
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(1.f / FMath::Abs(100.f * Magnitude)));
	case ESetByCallerMagnitudeDisplayType::XOver100:
		return FText::Format(FText::FromString("{0}"), FText::AsNumber(Magnitude / 100.f));
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE



