#pragma once

#include "CardAttributeFragment_Base.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayCueInterface.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Cards/Augments/CardAugmentData.h"
#include "CardExecution_AdditionalData.h"

#include "CardAttributeFragment_Execution.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FExecutionCompletedDelegate, APawn*, const FCardExecution_AdditionalData&, class UCardEffectFragment_Execution*);

UCLASS(Abstract, BlueprintType)
class UCardExecutionInstance : public UObject
{
	GENERATED_BODY()

public:
	FExecutionCompletedDelegate OnExecutionCompleted;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APawn> ExecutingPawn;

	UPROPERTY(BlueprintReadOnly)
	FCardExecution_AdditionalData AdditionalData;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCardEffectFragment_Execution> Exec;

	void StartExecutionInstance(APawn* InExecutingPawn, const FCardExecution_AdditionalData& InAdditionalData, class UCardEffectFragment_Execution* InExec);

protected:
	virtual void ProcessExecutionInstance() {};
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
	UCardExecutionInstance* ExecuteCardEffect(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData, UCardEffectFragment_Execution* ParentExecution = nullptr);

	UPROPERTY(Transient)
	TObjectPtr<UCardEffectFragment_Execution_Compound> ParentCompound;

	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
	{
		return GetLocalizableName_Implementation(CardIdentity);
	}

protected:
	TSubclassOf<UCardExecutionInstance> ExecInstanceClass = nullptr;

private:
	UPROPERTY()
	TArray<TObjectPtr<UCardExecutionInstance>> RunningInstances;

	UPROPERTY()
	TObjectPtr<UCardEffectFragment_Execution> ParentExec = nullptr;
};



UCLASS(Abstract)
class UCardEffectFragment_Execution_WithParameters : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TArray<UCardParameter*> Parameters;

	UCardExecutionInstance* ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData, UCardEffectFragment_Execution* ParentExecution) override;

	virtual void GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity);
	virtual void GatherParameters(FCardExecution_AdditionalData& OutData) const;
};

UCLASS()
class UCardExecutionInstance_Compound : public UCardExecutionInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<TObjectPtr<UCardExecutionInstance>> RunningSubInstances;
};


UCLASS(BlueprintType, Blueprintable, Abstract, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_Execution_Compound : public UCardEffectFragment_Execution_WithParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Compound")
	TArray<TObjectPtr<UCardEffectFragment_Execution>> Executions;

	UFUNCTION(BlueprintPure)
	void FindSubExecutionsOfClass(TSubclassOf<UCardEffectFragment_Execution> Class, TArray<UCardEffectFragment_Execution*>& OutArray);

protected:
	virtual void PostLoad();
};
