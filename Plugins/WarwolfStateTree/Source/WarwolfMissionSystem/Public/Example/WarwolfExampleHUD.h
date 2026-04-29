// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarwolfMissionUiInterface.h"
#include "GameFramework/HUD.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfExampleHUD.generated.h"

class UWarwolfExampleMissionWidget;

/**
 * 
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API AWarwolfExampleHUD : public AHUD, public IWarwolfMissionUiInterface
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	// IWarwolfMissionUiInterface
	virtual void ToastMission_Implementation(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType) override;
	virtual void ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType) override;
	virtual void ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid) override;
	virtual void HideObjective_Implementation(const FGuid InGuid) override;
	virtual void HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid) override;
	virtual void UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount) override;
	virtual void UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime) override;
	// ~IWarwolfMissionUiInterface

	// @brief Add mission HUD to viewport.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void ShowMissionHud();
	
	
protected:
	
	// @brief Class of the widget we want to use for our mission widget.
    UPROPERTY(EditDefaultsOnly, Category = "WarwolfMissionSystem")
    TSubclassOf<UWarwolfExampleMissionWidget> MissionWidgetClass = nullptr;

	// @brief String table used for updating widgets.
	UPROPERTY(EditDefaultsOnly, Category = "WarwolfMissionSystem")
	TSoftObjectPtr<UStringTable> StringTable;

	// @brief Right text tags for mission toast.
	UPROPERTY(EditDefaultsOnly, Category = "WarwolfMissionSystem")
	FText MissionToastTextTags = FText();
    
	// @brief Widget for displaying mission info.
	UPROPERTY(BlueprintReadOnly, Category = "WarwolfMissionSystem")
	TObjectPtr<UWarwolfExampleMissionWidget> MissionWidget = nullptr;
	
};
