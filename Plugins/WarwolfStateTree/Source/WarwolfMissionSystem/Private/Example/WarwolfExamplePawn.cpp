// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/WarwolfExamplePawn.h"

#include "WarwolfMissionTagComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWarwolfExamplePawn::AWarwolfExamplePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MissionTagComponent = CreateDefaultSubobject<UWarwolfMissionTagComponent>(TEXT("MissionTagComponent"));
}

// Called when the game starts or when spawned
void AWarwolfExamplePawn::BeginPlay()
{
	Super::BeginPlay();

	if (bActivateByDefault)
	{
		IWarwolfMissionTaskInterface::Execute_ActivateMissionActor(this);
		return;
	}

	IWarwolfMissionTaskInterface::Execute_DeactivateMissionActor(this);
}

// Called every frame
void AWarwolfExamplePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarwolfExamplePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWarwolfExamplePawn, bIsActivated);
	DOREPLIFETIME(AWarwolfExamplePawn, bActivateByDefault);
}

// Called to bind functionality to input
void AWarwolfExamplePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarwolfExamplePawn::ActivateMissionActor_Implementation()
{
	bIsActivated = true;
}

void AWarwolfExamplePawn::DeactivateMissionActor_Implementation()
{
	bIsActivated = false;
}

void AWarwolfExamplePawn::CountTowardMission()
{
	OnIntCountDelegate.Broadcast();
}

