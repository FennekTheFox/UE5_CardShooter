// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "WarwolfMissionTagComponent.generated.h"

/*
 * Component we place on any actor we want to interact with the mission system in some way.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARWOLFMISSIONSYSTEM_API UWarwolfMissionTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWarwolfMissionTagComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// @brief Get MissionTags tag container.
	// @return The mission tags currently assigned to this component.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	FGameplayTagContainer GetMissionTags() { return MissionTags; }

	// @brief Set MissionTags tag container.
	// @param The mission tags we want to assign to this component.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void SetMissionTags(const FGameplayTagContainer InTags) { MissionTags = InTags; }

protected:

	// @brief Mission related tags we want to identify the parent actor by.
	UPROPERTY(EditAnywhere, Category = "WarwolfMissionSystem")
	FGameplayTagContainer MissionTags = FGameplayTagContainer();
};
