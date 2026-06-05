#include "AnimModifier_SyncMarkerCueNotifies.h"


#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequence.h"
#include "AnimationBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimModifier_SyncMarkerCueNotifies)

UAnimModifier_SyncMarkerCueNotifies::UAnimModifier_SyncMarkerCueNotifies()
	: Super()
{
	NotifyTrackName = TEXT("Sync Marker Cues");
	bShouldRemovePreExistingNotifiesOnTrack = false;
	bGeneratedNotifyTrack = false;
}

void UAnimModifier_SyncMarkerCueNotifies::OnApply_Implementation(UAnimSequence* InAnimation)
{
	Super::OnApply_Implementation(InAnimation);

	if (!InAnimation)
	{
		UE_LOG(LogAnimation, Error, TEXT("SyncMarkerCueNotifies failed. Reason: Invalid Animation"));
		return;
	}

	if (NotifyTrackName.IsNone())
	{
		UE_LOG(LogAnimation, Error, TEXT("SyncMarkerCueNotifies failed. Reason: Invalid NotifyTrackName"));
		return;
	}

	ValidateNotifyTrack(InAnimation);

	const float SequenceLength = InAnimation->GetPlayLength();

	for (const FSyncMarkerCueNotifyMapping& Mapping : SyncMarkerNotifyMappings)
	{
		if (Mapping.SyncMarkerName.IsNone())
		{
			UE_LOG(LogAnimation, Warning, TEXT("SyncMarkerCueNotifies skipped mapping. Reason: Invalid SyncMarkerName"));
			continue;
		}

		if (!Mapping.NotifyClass)
		{
			UE_LOG(LogAnimation, Warning, TEXT("SyncMarkerCueNotifies skipped mapping for marker '%s'. Reason: Invalid NotifyClass"),
				*Mapping.SyncMarkerName.ToString());
			continue;
		}

		for (const FSyncMarkerCueNotifyMapping& SyncMarkerMapping : SyncMarkerNotifyMappings)
		{
			if (SyncMarkerMapping.SyncMarkerName.IsNone() || !SyncMarkerMapping.NotifyClass)
			{
				continue;
			}

			for (const FAnimSyncMarker& SyncMarker : InAnimation->AuthoredSyncMarkers)
			{
				if (SyncMarker.MarkerName != SyncMarkerMapping.SyncMarkerName)
				{
					continue;
				}

				const float NotifyTime = FMath::Clamp(
					SyncMarker.Time + SyncMarkerMapping.TimeOffset,
					0.0f,
					SequenceLength
				);

				UAnimationBlueprintLibrary::AddAnimationNotifyEvent(
					InAnimation,
					NotifyTrackName,
					NotifyTime,
					SyncMarkerMapping.NotifyClass
				);
			}
		}
	}
}

void UAnimModifier_SyncMarkerCueNotifies::OnRevert_Implementation(UAnimSequence* InAnimation)
{
	Super::OnRevert_Implementation(InAnimation);

	if (!InAnimation)
	{
		return;
	}

	if (bGeneratedNotifyTrack)
	{
		UAnimationBlueprintLibrary::RemoveAnimationNotifyTrack(InAnimation, NotifyTrackName);
	}
	else if (UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(InAnimation, NotifyTrackName))
	{
		for (const FSyncMarkerCueNotifyMapping& Mapping : SyncMarkerNotifyMappings)
		{
			if (!Mapping.NotifyClass)
			{
				continue;
			}

			const UAnimNotify* NotifyCDO = Cast<UAnimNotify>(Mapping.NotifyClass->GetDefaultObject());

			if (!NotifyCDO)
			{
				UE_LOG(LogAnimation, Error, TEXT("SyncMarkerCueNotifies failed to revert. Reason: Invalid UAnimNotify class"));
				continue;
			}

			UAnimationBlueprintLibrary::RemoveAnimationNotifyEventsByName(
				InAnimation,
				FName(NotifyCDO->GetNotifyName())
			);
		}
	}

	bGeneratedNotifyTrack = false;
}

void UAnimModifier_SyncMarkerCueNotifies::ValidateNotifyTrack(UAnimSequence* InAnimation)
{
	check(InAnimation);

	bGeneratedNotifyTrack = false;

	PrepareNotifyTrack(InAnimation);
}

void UAnimModifier_SyncMarkerCueNotifies::PrepareNotifyTrack(UAnimSequence* InAnimation)
{
	const bool bDoesTrackAlreadyExist =
		UAnimationBlueprintLibrary::IsValidAnimNotifyTrackName(InAnimation, NotifyTrackName);

	if (!bDoesTrackAlreadyExist)
	{
		UAnimationBlueprintLibrary::AddAnimationNotifyTrack(
			InAnimation,
			NotifyTrackName,
			FLinearColor::MakeRandomColor()
		);

		bGeneratedNotifyTrack = true;
	}
	else if (bShouldRemovePreExistingNotifiesOnTrack)
	{
		UAnimationBlueprintLibrary::RemoveAnimationNotifyEventsByTrack(
			InAnimation,
			NotifyTrackName
		);
	}
}