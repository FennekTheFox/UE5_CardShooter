// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Example/WarwolfExampleActor.h"
#include "Components/BoxComponent.h"
#include "WarwolfExampleTestPickup.generated.h"

class UWarwolfMissionTagComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogWarwolfMissionTest, Log, All);

UCLASS()
class WARWOLFMISSIONSYSTEM_API AWarwolfExampleTestPickup : public AWarwolfExampleActor
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	AWarwolfExampleTestPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// AWarwolfExampleActor
	void ActivateMissionActor_Implementation() override;
	void DeactivateMissionActor_Implementation() override;
	// ~AWarwolfExampleActor

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// @brief When overlapped, broadcast a delegate for state tree tasks to (if bound) to count.
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "WarwolfMissionSystem")
	void OverlappedPlayer(APawn* OverlappedPawn);

	// @brief Box collision component to handle overlaps.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	
};