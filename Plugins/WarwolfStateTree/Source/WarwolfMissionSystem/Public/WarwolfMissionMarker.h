// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
//
#include "WarwolfMissionMarkerWidget.h"
#include "WarwolfMissionTagComponent.h"
#include "GameFramework/Actor.h"
#include "WarwolfMissionTagInterface.h"
#include "WarwolfMissionTypes.h"
//
#include "WarwolfMissionMarker.generated.h"

/*
 * Actor class responsible for displaying a widget where a designer wants. Supports activation/deactivation
 * from state tree tasks.
*/

UCLASS()
class WARWOLFMISSIONSYSTEM_API AWarwolfMissionMarker : public AActor, public IWarwolfMissionTagInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarwolfMissionMarker();

	// IWarwolfMissionTagInterface
	virtual FGameplayTagContainer GetMissionTags_Implementation() override;
	// ~IWarwolfMissionTagInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// @brief Activate this marker and show it on the HUD.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void ActivateMarker();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ActivateMarker();

	// @brief Deactivate this marker and hide it on the HUD.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void DeactivateMarker();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeactivateMarker();

	// @brief Does this marker start active?
	UPROPERTY(EditAnywhere, Category = "WarwolfMissionSystem")
	bool bStartsActive = false;

	// @brief Text to display on the marker
	UPROPERTY(EditAnywhere, Category = "WarwolfMissionSystem")
	FText MarkerText = FText();

	// @brief Class of the widget we want to use for this marker. i.e. its appearance.
	UPROPERTY(EditInstanceOnly, Category = "WarwolfMissionSystem")
	TSubclassOf<UWarwolfMissionMarkerWidget> MarkerWidgetClass;

	// @brief What type of objective is this marker used for?
	UPROPERTY(EditInstanceOnly, Category = "WarwolfMissionSystem")
	EWarwolfMissionType MarkerType = EWarwolfMissionType::Primary;

	// @brief Mission tag component. Contains mission-related gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UWarwolfMissionTagComponent> TagComponent = nullptr;
	

protected:

	// @brief Is this marker active?
	UPROPERTY()
	bool bActive = bStartsActive;

	// @brief Widget for displaying this marker.
	UPROPERTY()
	TObjectPtr<UWarwolfMissionMarkerWidget> MarkerWidget = nullptr;

};
