// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/UI/WarwolfEMissionToastWidget.h"
#include "WarwolfMissionTypes.h"
#include "Components/RichTextBlock.h"
#include "Components/Border.h"
#include "Materials/MaterialInstanceDynamic.h"

void UWarwolfExampleMissionToastWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.f);
}


void UWarwolfExampleMissionToastWidget::ShowMissionToast(const FText InToastName, const FText InMissionName, const EWarwolfMissionType InMissionType)
{
	if(!ToastBegin) return;
	if(!MissionNameText) return;
	if(!ToastText) return;

	ToastText->SetText(InToastName);
	MissionNameText->SetText(InMissionName);

	SetRenderOpacity(1.f);

	PlayAnimation(ToastBegin, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	
	UE_LOG(LogWarwolfMissionUI, Log, TEXT("%s Showing toast for mission %s"), *FString(__FUNCTION__), *InMissionName.ToString());
}