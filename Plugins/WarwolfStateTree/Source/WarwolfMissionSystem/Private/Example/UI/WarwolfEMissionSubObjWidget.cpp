// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/UI/WarwolfEMissionSubObjWidget.h"
#include "Components/RichTextBlock.h"


void UWarwolfExampleMissionSubObjWidget::SetSubObjectiveText(const FText InSubObjText)
{
	if (!SubObjText) return;

	SubObjectiveText = InSubObjText;

	SubObjText->SetText(SubObjectiveText);
}

void UWarwolfExampleMissionSubObjWidget::HideSubObjective()
{
	if (!SubObjFinishedAnim) return;
	if (!SubObjAppearAnim) return;

	if (IsAnimationPlaying(SubObjAppearAnim)) StopAnimation(SubObjAppearAnim);

	FinishAnimDelegate.BindDynamic(this, &UWarwolfExampleMissionSubObjWidget::RemoveFromParent);
	BindToAnimationFinished(SubObjFinishedAnim, FinishAnimDelegate);

	PlayAnimation(SubObjFinishedAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
}

void UWarwolfExampleMissionSubObjWidget::PlayAppearanceAnim()
{
	if (!SubObjAppearAnim) return;
	
	PlayAnimation(SubObjAppearAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
}

void UWarwolfExampleMissionSubObjWidget::UpdateCount(const int32 InCurrentCount, const int32 InTotalCount) const
{
	if (!SubObjText) return;

	const FText UpdatedCount = FText::Format(SubObjectiveText, InCurrentCount, InTotalCount);

	SubObjText->SetText(UpdatedCount);
}

void UWarwolfExampleMissionSubObjWidget::UpdateTime(const float InCurrentTime, const float InTotalTime) const
{
	if (!SubObjText) return;

	const int32 CurrentMinutes = FMath::CeilToInt(InCurrentTime / 60.f);
	const int32 CurrentSeconds = FMath::CeilToInt(InCurrentTime) % 60;
	const int32 TotalMinutes = FMath::CeilToInt(InCurrentTime / 60.f);
	const int32 TotalSeconds = FMath::CeilToInt(InCurrentTime) % 60;
	const FText CurrentTimeText = FText::Format(FText::FromString("{0}:{1}"), CurrentMinutes, CurrentSeconds);
	const FText TotalTimeText = FText::Format(FText::FromString("{0}:{1}"), TotalMinutes, TotalSeconds);

	const FText UpdatedTime = FText::Format(SubObjectiveText, CurrentTimeText, TotalTimeText);

	SubObjText->SetText(UpdatedTime);
}
