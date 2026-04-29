// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "WarwolfMissionMarker.h"
#include "WarwolfMissionMarkerWidget.h"
#include "WarwolfMissionTypes.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWarwolfMissionMarker::AWarwolfMissionMarker()
	: TagComponent(CreateDefaultSubobject<UWarwolfMissionTagComponent>(TEXT("Warwolf Mission Tag Component")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AWarwolfMissionMarker::BeginPlay()
{
	Super::BeginPlay();

	if(!bStartsActive) return;

	ActivateMarker();

	if(!MarkerWidget)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s MarkerWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	MarkerWidget->UpdateMarkerName(MarkerText);
	MarkerWidget->UpdateMarkerLocation(GetActorLocation());
	MarkerWidget->UpdateMarkerType(MarkerType);
}

// Called every frame
void AWarwolfMissionMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bActive) return;

	if(!MarkerWidget)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s MarkerWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0 );
	if(!PlayerController)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	FVector CameraLocation;
	FRotator CameraRotation;

	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const float Distance = FVector::Distance(GetActorLocation(), CameraLocation);
	FText DistanceText;
	
	if (Distance >= 100000.0f) // KM
	{
		DistanceText = FText::Format(NSLOCTEXT("WarwolfMission", "Kilometre", "{0} km"), FText::AsNumber(Distance / 100000.0f, &FNumberFormattingOptions().SetMaximumFractionalDigits(0)));
	}
	else if (Distance >= 100.0f) // M
	{
		DistanceText = FText::Format(NSLOCTEXT("WarwolfMission", "Metres", "{0} m"), FText::AsNumber(Distance / 100.0f, &FNumberFormattingOptions().SetMaximumFractionalDigits(0)));
	}
	else // Less than 1 meter
	{
		DistanceText = FText::Format(NSLOCTEXT("WarwolfMission", "Metres", "{0} m"), FText::AsNumber(Distance / 100.0f, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
	}

	MarkerWidget->UpdateMarkerDistance(DistanceText);
	MarkerWidget->UpdateMarkerLocation(GetActorLocation());
	MarkerWidget->UpdateMarkerName(MarkerText);
	MarkerWidget->UpdateMarkerType(MarkerType);
}

void AWarwolfMissionMarker::ActivateMarker()
{
	if (!HasAuthority()) return;

	Multicast_ActivateMarker();
}

void AWarwolfMissionMarker::Multicast_ActivateMarker_Implementation()
{
	if (bActive) return;
	bActive = true;
	
	if (!MarkerWidgetClass)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s MarkerWidgetClass is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0 );
	if(!PlayerController)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	MarkerWidget = CreateWidget<UWarwolfMissionMarkerWidget>(PlayerController, MarkerWidgetClass);

	if (!MarkerWidget)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s MarkerWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	MarkerWidget->AddToViewport();
	MarkerWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AWarwolfMissionMarker::DeactivateMarker()
{
	if (!HasAuthority()) return;

	Multicast_DeactivateMarker();
}

void AWarwolfMissionMarker::Multicast_DeactivateMarker_Implementation()
{
	bActive = false;
	
	if (!MarkerWidget)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s MarkerWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	MarkerWidget->RemoveFromParent();
}

FGameplayTagContainer AWarwolfMissionMarker::GetMissionTags_Implementation()
{
	if(!IsValid(TagComponent))
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s TagComponent is not valid. Returning empty container."), *FString(__FUNCTION__));
		return FGameplayTagContainer();
	}

	return TagComponent->GetMissionTags();
}

