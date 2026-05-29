#include "AnimModifier_SyncMarkerCueNotifies.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"

void UAnimModifier_SyncMarkerCueNotifies::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (!AnimationSequence)
	{
		return;
	}

	AnimationSequence->Modify();

	if (bClearExistingGeneratedNotifies)
	{
		RemoveGeneratedNotifies(AnimationSequence);
	}

	const int32 TrackIndex = FindOrAddNotifyTrack(AnimationSequence);

	for (const FAnimSyncMarker& SyncMarker : AnimationSequence->AuthoredSyncMarkers)
	{
		const FSyncMarkerCueNotifyMapping* Mapping = FindMapping(SyncMarker.MarkerName);

		if (!Mapping || !Mapping->NotifyClass)
		{
			continue;
		}

		const float MarkerTime = SyncMarker.Time;
		const UClass* DesiredNotifyClass = Mapping->NotifyClass.Get();

		const bool bAlreadyExists = AnimationSequence->Notifies.ContainsByPredicate(
			[MarkerTime, DesiredNotifyClass, this](const FAnimNotifyEvent& ExistingEvent)
			{
				return ExistingEvent.Notify
					&& ExistingEvent.Notify->GetClass() == DesiredNotifyClass
					&& FMath::Abs(ExistingEvent.GetTime() - MarkerTime) <= DuplicateTimeTolerance;
			});

		if (bAlreadyExists)
		{
			continue;
		}

		UAnimNotify* NewNotify = NewObject<UAnimNotify>(
			AnimationSequence,
			DesiredNotifyClass,
			NAME_None,
			RF_Transactional
		);

		const FString NotifyNameString = FString::Printf(
			TEXT("%s%s_%s"),
			*GeneratedNotifyNamePrefix,
			*Mapping->FootDefinition.ToString(),
			*SyncMarker.MarkerName.ToString()
		);

		FAnimNotifyEvent NewEvent;
		NewEvent.Notify = NewNotify;
		NewEvent.NotifyName = FName(*NotifyNameString);
		NewEvent.TrackIndex = TrackIndex;

		NewEvent.Link(AnimationSequence, MarkerTime);
		NewEvent.TriggerTimeOffset =
			GetTriggerTimeOffsetForType(AnimationSequence->CalculateOffsetForNotify(MarkerTime));

		AnimationSequence->Notifies.Add(NewEvent);
	}

	AnimationSequence->RefreshCacheData();
	AnimationSequence->MarkPackageDirty();
}

void UAnimModifier_SyncMarkerCueNotifies::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	if (!AnimationSequence)
	{
		return;
	}

	AnimationSequence->Modify();

	RemoveGeneratedNotifies(AnimationSequence);

	AnimationSequence->RefreshCacheData();
	AnimationSequence->MarkPackageDirty();
}

const FSyncMarkerCueNotifyMapping* UAnimModifier_SyncMarkerCueNotifies::FindMapping(FName SyncMarkerLabel) const
{
	return MarkerMappings.FindByPredicate(
		[SyncMarkerLabel](const FSyncMarkerCueNotifyMapping& Mapping)
		{
			return Mapping.SyncMarkerLabel == SyncMarkerLabel;
		});
}

void UAnimModifier_SyncMarkerCueNotifies::RemoveGeneratedNotifies(UAnimSequence* AnimationSequence) const
{
	if (!AnimationSequence)
	{
		return;
	}

	AnimationSequence->Notifies.RemoveAll(
		[this](const FAnimNotifyEvent& Event)
		{
			return Event.Notify
				&& Event.NotifyName.ToString().StartsWith(GeneratedNotifyNamePrefix);
		});
}

int32 UAnimModifier_SyncMarkerCueNotifies::FindOrAddNotifyTrack(UAnimSequence* AnimationSequence) const
{
	check(AnimationSequence);

	const int32 ExistingTrackIndex = AnimationSequence->AnimNotifyTracks.IndexOfByPredicate(
		[this](const FAnimNotifyTrack& Track)
		{
			return Track.TrackName == NotifyTrackName;
		});

	if (ExistingTrackIndex != INDEX_NONE)
	{
		return ExistingTrackIndex;
	}

	FAnimNotifyTrack NewTrack;
	NewTrack.TrackName = NotifyTrackName;
	NewTrack.TrackColor = FLinearColor(0.05f, 0.35f, 1.0f).ToFColor(true);

	return AnimationSequence->AnimNotifyTracks.Add(NewTrack);
}