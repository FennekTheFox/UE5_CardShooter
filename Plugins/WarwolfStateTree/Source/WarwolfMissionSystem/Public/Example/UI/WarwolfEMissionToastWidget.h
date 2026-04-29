// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarwolfMissionTypes.h"
#include "Blueprint/UserWidget.h"
#include "WarwolfEMissionToastWidget.generated.h"

class URichTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfExampleMissionToastWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// @brief Show a toast on the mission UI.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void ShowMissionToast(const FText InToastName, const FText InMissionName, const EWarwolfMissionType InMissionType);

	// Widgets

	// @brief The animation which plays when hiding the prompt.
	UPROPERTY(Transient, BlueprintReadOnly, Category = "WarwolfMissionSystem", meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ToastBegin;

	// @brief Text that shows what type of toast this is.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<URichTextBlock> ToastText;

	// @brief Text that shows what the mission name is.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<URichTextBlock> MissionNameText;

	// @brief Border that shows the background colour.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UBorder> ColourBorder;

	// ~Widgets
	
};
