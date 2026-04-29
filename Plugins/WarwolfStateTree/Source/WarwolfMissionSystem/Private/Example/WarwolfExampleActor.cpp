// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/WarwolfExampleActor.h"

#include "Net/UnrealNetwork.h"
#include "WarwolfMissionTagComponent.h"

// Sets default values
AWarwolfExampleActor::AWarwolfExampleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MissionTagComponent = CreateDefaultSubobject<UWarwolfMissionTagComponent>(TEXT("MissionTagComponent"));
}

// Called when the game starts or when spawned
void AWarwolfExampleActor::BeginPlay()
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
void AWarwolfExampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWarwolfExampleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWarwolfExampleActor, bIsActivated);
	DOREPLIFETIME(AWarwolfExampleActor, bActivateByDefault);
}

void AWarwolfExampleActor::ActivateMissionActor_Implementation()
{
	bIsActivated = true;
}

void AWarwolfExampleActor::DeactivateMissionActor_Implementation()
{
	bIsActivated = false;
}

void AWarwolfExampleActor::CountTowardMission()
{
	OnIntCountDelegate.Broadcast();
}
