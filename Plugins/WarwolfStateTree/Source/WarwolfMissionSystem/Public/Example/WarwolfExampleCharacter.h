// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Tasks/WarwolfMissionTaskInterface.h"

#include "WarwolfExampleCharacter.generated.h"

class UWarwolfMissionTagComponent;

UCLASS()
class WARWOLFMISSIONSYSTEM_API AWarwolfExampleCharacter : public ACharacter, public IWarwolfMissionTaskInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarwolfExampleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// IWarwolfMissionTaskInterface
	// @brief Int count delegate for counting to the task.
	FOnWarwolfIntCount OnIntCountDelegate;
	virtual FOnWarwolfIntCount& GetIntCountDelegate() override { return OnIntCountDelegate; }
	virtual void ActivateMissionActor_Implementation() override;
	virtual void DeactivateMissionActor_Implementation() override;
	// ~IWarwolfMissionTaskInterface

	// @brief Call this when we want to count something in the mission system.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void CountTowardMission();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// @brief Mission tag component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWarwolfMissionTagComponent> MissionTagComponent;

	// @brief Should we activate the character by default?
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem", meta = (ExposeOnSpawn = "true"))
	bool bActivateByDefault = false;

protected:
	// @brief Is the character activated?
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "WarwolfMissionSystem")
	bool bIsActivated = false;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
