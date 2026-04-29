// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/WarwolfExampleCharacter.h"

#include "WarwolfMissionTagComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWarwolfExampleCharacter::AWarwolfExampleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	MissionTagComponent = CreateDefaultSubobject<UWarwolfMissionTagComponent>(TEXT("MissionTagComponent"));
}

// Called when the game starts or when spawned
void AWarwolfExampleCharacter::BeginPlay()
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
void AWarwolfExampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarwolfExampleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWarwolfExampleCharacter, bIsActivated);
	DOREPLIFETIME(AWarwolfExampleCharacter, bActivateByDefault);
}

// Called to bind functionality to input
void AWarwolfExampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarwolfExampleCharacter::ActivateMissionActor_Implementation()
{
	bIsActivated = true;
}

void AWarwolfExampleCharacter::DeactivateMissionActor_Implementation()
{
	bIsActivated = false;
}

void AWarwolfExampleCharacter::CountTowardMission()
{
	OnIntCountDelegate.Broadcast();
}
