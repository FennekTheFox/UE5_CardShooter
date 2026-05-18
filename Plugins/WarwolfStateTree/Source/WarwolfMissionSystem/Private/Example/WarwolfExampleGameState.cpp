// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/WarwolfExampleGameState.h"
#include "WarwolfMissionDataAsset.h"
#include "WarwolfMissionSystem/Public/WarwolfMissionManager.h"
#include "WarwolfMissionTypes.h"
#include "GameFramework/HUD.h"
#include "Engine/World.h"
#include "LevelSequenceActor.h"

DEFINE_LOG_CATEGORY(LogWarwolfGameState);

AWarwolfExampleGameState::AWarwolfExampleGameState() :
	MissionManager(CreateDefaultSubobject<UWarwolfMissionManager>(TEXT("Warwolf Mission Manager")))
{
	
}

void AWarwolfExampleGameState::StartMission_Implementation()
{
	UE_LOG(LogWarwolfGameState, Log, TEXT("%s Starting mission %s"), *FString(__FUNCTION__), *SelectedMission->MissionName.ToString());
	
	if (!IsValid(SelectedMission))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s SelectedMission is invalid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	const FName EventTagName = FName("Mission.Event.Start");
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(EventTagName);
	const FName Origin = FName("WarwolfGameState");

	if (!EventTag.IsValid())
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Couldn't find tag for 'Mission.Event.Start'. We've either hit a race condition with the tag dictionary, or the tag has been removed? Returning."), *FString(__FUNCTION__));
		return;
	}

	FStateTreeEvent StateTreeEvent;
	StateTreeEvent.Tag = EventTag;
	StateTreeEvent.Origin = Origin;
	
	if (!IsValid(MissionManager))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s MissionManager is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	if (!MissionManager->GetClass()->ImplementsInterface(UWarwolfMissionInterface::StaticClass()))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s MissionManager does not implement IWarwolfMissionInterface. Returning."), *FString(__FUNCTION__));
		return;
	}

	IWarwolfMissionInterface::Execute_SendMissionEvent(MissionManager, StateTreeEvent);
}

void AWarwolfExampleGameState::SelectMission_Implementation(UWarwolfMissionDataAsset* InMissionData, const bool bStartMissionInstantly)
{
	SelectedMission = InMissionData;

	if (!MissionManager)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s MissionManager is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	if (!IsValid(InMissionData->MissionStateTree))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Mission state tree in InMissionData is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	MissionManager->SetMissionStateTree(InMissionData->MissionStateTree);
	MissionManager->StartLogic();

	if(!bStartMissionInstantly) return;

	if (!this->GetClass()->ImplementsInterface(UWarwolfMissionInterface::StaticClass()))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s this does not implement IWarwolfMissionInterface. Returning."), *FString(__FUNCTION__));
		return;
	}

	IWarwolfMissionInterface::Execute_StartMission(this);
}

void AWarwolfExampleGameState::SendMissionEvent_Implementation(const FStateTreeEvent& Event)
{
	// Calling via interface to avoid linker issues.
	if (!IsValid(MissionManager))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s MissionManager is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	if (!MissionManager->GetClass()->ImplementsInterface(UWarwolfMissionInterface::StaticClass()))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s MissionManager does not implement IWarwolfMissionInterface. Returning."), *FString(__FUNCTION__));
		return;
	}

	IWarwolfMissionInterface::Execute_SendMissionEvent(MissionManager, Event);

	if (SelectedMission)
	{
		UE_LOG(LogWarwolfGameState, Log, TEXT("%s Starting mission event %s for mission %s"), *FString(__FUNCTION__), *Event.Tag.ToString(), *SelectedMission->MissionName.ToString());
	}
}

void AWarwolfExampleGameState::ToastMission_Implementation(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType)
{
	if (!HasAuthority()) return;

	Multicast_ToastMission(InMissionName, InMissionType, InToastType);
}

void AWarwolfExampleGameState::Multicast_ToastMission_Implementation(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	const FString MissionTypeString = UEnum::GetDisplayValueAsText(InMissionType).ToString();
	const FString ToastTypeString = UEnum::GetDisplayValueAsText(InToastType).ToString();

	UE_LOG(LogWarwolfGameState, Log, TEXT("%s Toasting mission %s with name %s and type %s"),
		   *FString(__FUNCTION__), *ToastTypeString, *InMissionName.ToString(), *MissionTypeString);

	InterfaceInstance->Execute_ToastMission(Hud, InMissionName, InMissionType, InToastType);
}

void AWarwolfExampleGameState::ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType)
{
	if (!HasAuthority()) return;

	Multicast_ShowObjective(InObjText, InGuid, InMissionType);
}

void AWarwolfExampleGameState::Multicast_ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	const FString MissionTypeString = UEnum::GetDisplayValueAsText(InMissionType).ToString();

	UE_LOG(LogWarwolfGameState, Log, TEXT("%s New GUID set for BoundGuid: %s for objective '%s'."), *FString(__FUNCTION__), *InGuid.ToString(), *InObjText.ToString());

	InterfaceInstance->Execute_ShowObjective(Hud, InObjText, InGuid, InMissionType);
}

void AWarwolfExampleGameState::ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid)
{
	if (!HasAuthority()) return;

	Multicast_ShowSubObjective(InSubObjText, InObjGuid, InGuid);
}

void AWarwolfExampleGameState::Multicast_ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	UE_LOG(LogWarwolfGameState, Log, TEXT("%s Adding sub-objective to widget with guid: %s for sub-objective '%s' with guid: %s."), *FString(__FUNCTION__), *InObjGuid.ToString(), *InSubObjText.ToString(), *InGuid.ToString());

	InterfaceInstance->Execute_ShowSubObjective(Hud, InSubObjText, InObjGuid, InGuid);
}

void AWarwolfExampleGameState::HideObjective_Implementation(const FGuid InGuid)
{
	if (!HasAuthority()) return;

	Multicast_HideObjective(InGuid);
}

void AWarwolfExampleGameState::Multicast_HideObjective_Implementation(const FGuid InGuid)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	UE_LOG(LogWarwolfGameState, Log, TEXT("%s Using Guid: %s to hide objective."), *FString(__FUNCTION__), *InGuid.ToString());

	InterfaceInstance->Execute_HideObjective(Hud, InGuid);
}

void AWarwolfExampleGameState::HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid)
{
	if (!HasAuthority()) return;

	Multicast_HideSubObjective(InObjGuid, InGuid);
}

void AWarwolfExampleGameState::Multicast_HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	UE_LOG(LogWarwolfGameState, Log, TEXT("%s Hiding sub-objective from widget with guid: %s for sub-objective with guid: %s."), *FString(__FUNCTION__), *InObjGuid.ToString(), *InGuid.ToString());

	InterfaceInstance->Execute_HideSubObjective(Hud, InObjGuid, InGuid);
}

void AWarwolfExampleGameState::UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount)
{
	if (!HasAuthority()) return;

	Multicast_UpdateSubObjective_IntCount(InObjGuid, InGuid, InCurrentCount, InTotalCount);
}

void AWarwolfExampleGameState::Multicast_UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	InterfaceInstance->Execute_UpdateSubObjective_IntCount(Hud, InObjGuid, InGuid, InCurrentCount, InTotalCount);
}

void AWarwolfExampleGameState::UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime)
{
	if (!HasAuthority()) return;

	Multicast_UpdateSubObjective_Timer(InObjGuid, InGuid, InCurrentTime, InTotalTime);
}

void AWarwolfExampleGameState::Multicast_UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	AHUD* Hud = PlayerController->GetHUD();
	if (!Hud)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s Hud is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(Hud);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	InterfaceInstance->Execute_UpdateSubObjective_Timer(Hud, InObjGuid, InGuid, InCurrentTime, InTotalTime);
}

ULevelSequencePlayer* AWarwolfExampleGameState::PlayLevelSequence_Implementation(ULevelSequence* InLevelSequence)
{
	if (!HasAuthority()) return nullptr;

	Multicast_PlayLevelSequence(InLevelSequence);

	return PlayLevelSequence_Internal(InLevelSequence);
}

void AWarwolfExampleGameState::Multicast_PlayLevelSequence_Implementation(ULevelSequence* InLevelSequence)
{
	if (HasAuthority()) return; // Return early so server does not play the sequence here. 
	
	PlayLevelSequence_Internal(InLevelSequence);
}

ULevelSequencePlayer* AWarwolfExampleGameState::PlayLevelSequence_Internal(ULevelSequence* InLevelSequence) const
{
	if (!IsValid(InLevelSequence))
	{
		UE_LOG(LogWarwolfGameState, Error, TEXT("%s InLevelSequence is not valid. Returning."), *FString(__FUNCTION__));
		return nullptr;
	}
	
	ALevelSequenceActor* SequenceActor = nullptr;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld()
			, InLevelSequence
			, FMovieSceneSequencePlaybackSettings()
			, SequenceActor
		);
	
	SequencePlayer->Play();

	return SequencePlayer;
}

FOnWarwolfIntCount& AWarwolfExampleGameState::GetIntCountDelegate()
{
	static FOnWarwolfIntCount NullDelegate;
	return NullDelegate;
}
