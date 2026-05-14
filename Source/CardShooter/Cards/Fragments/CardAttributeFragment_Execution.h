#pragma once

#include "CardAttributeFragment_Base.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayCueInterface.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "CardAttributeFragment_Execution.generated.h"


UENUM(BlueprintType)
enum class ESetByCallerMagnitudeDisplayType : uint8
{
	Flat = 0,
	Percent = 1,
	PercentOneMinus = 2,
	Absolute,
	AbsolutePercent,
	AbsolutePercentOneOver,
};

USTRUCT(BlueprintType)
struct FSetByCallerMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Magnitude = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESetByCallerMagnitudeDisplayType DisplayType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ParameterName;

	FText GetDisplayText() const;
};


USTRUCT(BlueprintType)
struct FReplicatedTargetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag TargetDataID;

	UPROPERTY(BlueprintReadWrite)
	FGameplayAbilityTargetDataHandle TargetData;
};

USTRUCT(BlueprintType)
struct FCardExecution_AdditionalData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 EchoIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 ComboIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	TArray<FReplicatedTargetData> TargetData;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> SetByCallerMetaDataTags;

	UPROPERTY(BlueprintReadOnly)
	TArray<FSetByCallerMetaData> SetByCallerMetaDataValues;
};

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
	UCardExecutionInstance* ExecuteCardEffect(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData);

	FText GetLocalizableName_Implementation() override
	{
		return GetLocalizableName_Implementation();
	}

private:
	UPROPERTY()
	TArray<TObjectPtr<UCardExecutionInstance>> RunningInstances;
};



UCLASS(Abstract)
class UCardEffectFragment_ExecutionWithParameters : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SetByCallerParameters")
	TMap<FGameplayTag, FSetByCallerMetaData> SetByCallerMagnitudes;

	void GetSetByCallerFormattingArgs(FFormatNamedArguments& Args);
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
class UCardEffectFragment_Execution_Compound : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Compound")
	TArray<TObjectPtr<UCardEffectFragment_Execution>> Executions;

	UFUNCTION(BlueprintPure)
	void FindSubExecutionsOfClass(TSubclassOf<UCardEffectFragment_Execution> Class, TArray<UCardEffectFragment_Execution*>& OutArray);

protected:
};
