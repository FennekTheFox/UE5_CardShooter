// Copyright (c) 2025 Michael Greenard. All rights reserved.

#include "Example/WarwolfExampleTestPickup.h"
#include "Components/BoxComponent.h"
#include "WarwolfMissionTagComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

DEFINE_LOG_CATEGORY(LogWarwolfMissionTest);

// Sets default values
AWarwolfExampleTestPickup::AWarwolfExampleTestPickup()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	check(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = BoxComponent;
}

// Called when the game starts or when spawned
void AWarwolfExampleTestPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWarwolfExampleTestPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWarwolfExampleTestPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (!IsValid(OverlappedPawn)
		|| !HasAuthority()
		|| !bIsActivated)
		return;

	OnIntCountDelegate.Broadcast();
	OverlappedPlayer(OverlappedPawn);
}

void AWarwolfExampleTestPickup::ActivateMissionActor_Implementation()
{
	Super::ActivateMissionActor_Implementation();
	
	if (!HasAuthority()) return;
	
	if (!IsValid(BoxComponent))
	{
		UE_LOG(LogWarwolfMissionTest, Error, TEXT("%s BoxComponent is invalid. Returning."), *FString(__FUNCTION__));
		return;
	}

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	if (BoxComponent->OnComponentBeginOverlap.IsAlreadyBound(this, &AWarwolfExampleTestPickup::OnOverlapBegin))
	{
		UE_LOG(LogWarwolfMissionTest, Error, TEXT("%s BoxComponent overlaps are already bound. Why are we trying to do this again? Returning."), *FString(__FUNCTION__));
		return;
	}
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarwolfExampleTestPickup::OnOverlapBegin);
}

void AWarwolfExampleTestPickup::DeactivateMissionActor_Implementation()
{
	Super::DeactivateMissionActor_Implementation();
	
	if (!HasAuthority()) return;
	
	if (!IsValid(BoxComponent))
	{
		UE_LOG(LogWarwolfMissionTest, Error, TEXT("%s BoxComponent is invalid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AWarwolfExampleTestPickup::OnOverlapBegin);
}

void AWarwolfExampleTestPickup::OverlappedPlayer_Implementation(APawn* OverlappedPawn)
{
	
}

