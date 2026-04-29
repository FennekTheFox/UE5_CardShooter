#pragma once

#include "CardAttributeFragment_Base.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayCueInterface.h"

#include "CardAttributeFragment_Execution.generated.h"

USTRUCT(BlueprintType)
struct FCardExecution_AdditionalData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 EchoIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 ComboIndex = 0;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Execution : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:
	//Execution of cards always needs to happen on server side, and be replicated to the client
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ExecuteCardEffect(APawn* ExecutingPawn, const FCardExecution_AdditionalData AdditionalData = FCardExecution_AdditionalData());

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ExecuteCardEffect_Delayed(APawn* ExecutingPawn, const FCardExecution_AdditionalData Data, float Delay = 1.f);

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteCardEffect(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData);

	FText GetLocalizableName_Implementation() override
	{
		return GetLocalizableName_Implementation();	
	}

};

UCLASS(BlueprintType, Blueprintable, Abstract, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_Execution_Compound : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UCardEffectFragment_Execution>> Executions;

	UFUNCTION(BlueprintPure)
	void FindSubExecutionsOfClass(TSubclassOf<UCardEffectFragment_Execution> Class, TArray<UCardEffectFragment_Execution*>& OutArray);
};
