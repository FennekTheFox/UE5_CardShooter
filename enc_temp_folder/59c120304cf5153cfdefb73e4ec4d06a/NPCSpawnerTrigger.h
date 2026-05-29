#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "NPCSpawnerTrigger.generated.h"

class ANPCSpawner;
class UShapeComponent;
class UBoxComponent;

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class UNPCSpawnerTrigger : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(ANPCSpawner* InSpawner);
	virtual void Shutdown();

protected:
	UPROPERTY()
	TObjectPtr<ANPCSpawner> Spawner;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class UNPCSpawnerBeginPlayTrigger : public UNPCSpawnerTrigger
{
	GENERATED_BODY()

public:
	virtual void Initialize(ANPCSpawner* InSpawner) override;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class UNPCSpawnerEventTrigger : public UNPCSpawnerTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bMatchExactOnly = true;

	virtual void Initialize(ANPCSpawner* InSpawner) override;

	UFUNCTION()
	void Callback(FGameplayEventData Data);
};



UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class UNPCSpawnerBoxOverlapTrigger : public UNPCSpawnerTrigger
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	FVector BoxExtent = FVector(300.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	FVector RelativeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TSubclassOf<AActor> RequiredActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	bool bTriggerOnce = true;

	virtual void Initialize(ANPCSpawner* InSpawner) override;
	virtual void Shutdown() override;

private:
	UPROPERTY()
	TObjectPtr<UBoxComponent> BoxComponent;

	bool bHasTriggered = false;

	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};