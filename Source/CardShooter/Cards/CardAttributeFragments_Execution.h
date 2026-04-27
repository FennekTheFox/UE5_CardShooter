#pragma once

#include "CardAttributeFragment_Base.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayCueInterface.h"

#include "CardAttributeFragments_Execution.generated.h"

USTRUCT(BlueprintType)
struct FCardExecution_AdditionalData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 IterationCount = 0;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Execution : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:
	//Execution of cards always needs to happen on server side, and be replicated to the client
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ExecuteCardEffect(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData = FCardExecution_AdditionalData());

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ExecuteCardEffect_Delayed(APawn* ExecutingPawn, float Delay = 1.f, int32 IterationCount = 1);

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteCardEffect(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData = FCardExecution_AdditionalData());
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_Execution_GameplayAbility : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAbilitySpecDef AbilityToExecute;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData) override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_Execution_Compound : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UCardEffectFragment_Execution>> Executions;

	UFUNCTION(BlueprintPure)
	void FindSubExecutionsOfClass(TSubclassOf<UCardEffectFragment_Execution> Class, TArray<UCardEffectFragment_Execution*>& OutArray);
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Debug Print"))
class UCardEffectFragment_Execution_DebugPrint : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString Message;

	UPROPERTY(EditDefaultsOnly)
	FColor DisplayColor = FColor::Green;

	UPROPERTY(EditDefaultsOnly)
	float DisplayDuration = 2.f;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData) override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Apply Effect To Self"))
class UCardEffectFragment_Execution_ApplyEffectToOwner : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float GameplayEffectLevel = 1.f;

	//The tag applied by the gameplay effect to track when the persistent effect gets removed
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag TrackingTag;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData) override;
};