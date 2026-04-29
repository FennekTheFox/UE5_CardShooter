// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/WarwolfExampleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Example/UI/WarwolfEMissionWidget.h"
#include "WarwolfMissionTypes.h"

void AWarwolfExampleHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowMissionHud();
}


void AWarwolfExampleHUD::ToastMission_Implementation(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType)
{
	if(!MissionWidget)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const FSoftObjectPath StringTablePath = StringTable.ToSoftObjectPath();
	if (!StringTablePath.IsValid())
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s StringTablePath is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	FText LocalisedText = FText::FromString("Empty");
	FTextKey KeyName;
	const FString AssetPathString = StringTablePath.GetAssetPathString(); // Convert to string and _then_ FName as Unreal has deprecated converting it directly, but still wants me to use FName for FromStringTable. Sigh.
	const FName Namespace = *AssetPathString;

	switch (InToastType)
	{
	case EWarwolfMissionToastType::MissionStart:
		KeyName = FTextKey("Mission_Toast_Start");
		break;
    
	case EWarwolfMissionToastType::MissionUpdate:
		KeyName =  FTextKey("Mission_Toast_Update");
		break;
    
	case EWarwolfMissionToastType::MissionSuccess:
		KeyName =  FTextKey("Mission_Toast_Success");
		break;
    
	case EWarwolfMissionToastType::MissionFailure:
		KeyName =  FTextKey("Mission_Toast_Failure");
		break;
		
	case EWarwolfMissionToastType::ObjectiveStart:
		KeyName =  FTextKey("Objective_Toast_Start");
		break;
    
	case EWarwolfMissionToastType::ObjectiveUpdate:
		KeyName =  FTextKey("Objective_Toast_Update");
		break;
    
	case EWarwolfMissionToastType::ObjectiveSuccess:
		KeyName = FTextKey("Objective_Toast_Success");
		break;
    
	case EWarwolfMissionToastType::ObjectiveFailure:
		KeyName = FTextKey("Objective_Toast_Failure");
		break;

	default:
		KeyName = FTextKey("Invalid");
		break;
	}

	LocalisedText = FText::FromStringTable(Namespace, KeyName);

	if (LocalisedText.IsEmpty())
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Missing string entry for key: %s"), *FString(__FUNCTION__), *KeyName.ToString());
	}

	const FText FinalText = FText::Format(FText::FromString("<{0}>{1}</>"), MissionToastTextTags, LocalisedText);

	MissionWidget->ShowMissionToast(FinalText, InMissionName, InMissionType);
}

void AWarwolfExampleHUD::ShowMissionHud()
{
	if (!MissionWidgetClass)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidgetClass is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0 );
	if(!IsValid(PlayerController))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	MissionWidget = CreateWidget<UWarwolfExampleMissionWidget>(PlayerController, MissionWidgetClass);

	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	MissionWidget->AddToViewport();
}

void AWarwolfExampleHUD::ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType)
{
	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(MissionWidget);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	InterfaceInstance->Execute_ShowObjective(MissionWidget, InObjText, InGuid, InMissionType);
}

void AWarwolfExampleHUD::ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid)
{
	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(MissionWidget);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	InterfaceInstance->Execute_ShowSubObjective(MissionWidget, InSubObjText, InObjGuid, InGuid);
}

void AWarwolfExampleHUD::HideObjective_Implementation(const FGuid InGuid)
{
	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(MissionWidget);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	InterfaceInstance->Execute_HideObjective(MissionWidget, InGuid);
}

void AWarwolfExampleHUD::HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid)
{
	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(MissionWidget);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	InterfaceInstance->Execute_HideSubObjective(MissionWidget, InObjGuid, InGuid);
}

void AWarwolfExampleHUD::UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount)
{
	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(MissionWidget);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	InterfaceInstance->Execute_UpdateSubObjective_IntCount(MissionWidget, InObjGuid, InGuid, InCurrentCount, InTotalCount);
}

void AWarwolfExampleHUD::UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime)
{
	if (!IsValid(MissionWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const IWarwolfMissionUiInterface* InterfaceInstance = Cast<IWarwolfMissionUiInterface>(MissionWidget);
	if (!InterfaceInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s InterfaceInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	InterfaceInstance->Execute_UpdateSubObjective_Timer(MissionWidget, InObjGuid, InGuid, InCurrentTime, InTotalTime);
}

