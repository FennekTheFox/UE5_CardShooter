#include "GameplayAbility_CardExecution.h"

#include "Gameplay/GameplayEffect_CardExecution.h"
#include "AbilityTriggers.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"

#define TC_ONGRANTED UAbilityActivationTrigger_OnAbilityGranted

#define LOCTEXT_NAMESPACE "CardEffects"

UGameplayAbility_CardExecution::UGameplayAbility_CardExecution()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	ActivationTriggers.Add(Cast<TC_ONGRANTED>(TC_ONGRANTED::StaticClass()->GetDefaultObject(false)));
}

float UGameplayAbility_CardExecution::GetSetByCallerValue(FGameplayTag Tag)
{
	//See if this was directly executed. then we dont need to look through the context
	int32 Index = AdditionalData.SetByCallerMetaDataTags.Find(Tag);

	if (Index != INDEX_NONE)
	{
		return AdditionalData.SetByCallerMetaDataValues[Index].Magnitude;
	}

	//The source object could either be an instance, if the ability was granted by a GE which was applied via a card execution, or 
	//an ability execution if applied via an effect that was applied via an ability (yes i know its confusing). Either way we need
	//to retrieve the set by caller data from the _WithParameters fragment

	TMap<FGameplayTag, FSetByCallerMetaData>* SetByCallerData = nullptr;
	UCardEffectFragment_Execution_WithParameters* WithParams = nullptr;

	FGameplayEffectContextHandle Context = GetGrantedByEffectContext();
	UObject* ContextSourceObject = Context.GetSourceObject();
	if ((ContextSourceObject))
	{
		WithParams = Cast<UCardEffectFragment_Execution_WithParameters>(ContextSourceObject);

		if (!WithParams)
		{
			UCardExecutionInstance* AsInstance = Cast<UCardExecutionInstance>(ContextSourceObject);

			if (AsInstance)
			{
				WithParams = Cast<UCardEffectFragment_Execution_WithParameters>(AsInstance->Exec);
			}
		}
	}

	//If added directly via the CEF_GA, it should be the Source Object
	if (!WithParams)
	{
		UCardExecutionInstance* AsInstance = Cast<UCardExecutionInstance>(GetCurrentSourceObject());

		if (AsInstance)
		{
			WithParams = Cast<UCardEffectFragment_Execution_WithParameters>(AsInstance->Exec);
		}
	}		
	
	if (WithParams)
	{
		SetByCallerData = &(WithParams->SetByCallerMagnitudes);
	}

	if (ensure(SetByCallerData))
	{
		FSetByCallerMetaData* Data = WithParams->SetByCallerMagnitudes.Find(Tag);

		if (Data)
		{			
			return Data->Magnitude;
		}
	}	

	return 0.f;
}


#undef LOCTEXT_NAMESPACE