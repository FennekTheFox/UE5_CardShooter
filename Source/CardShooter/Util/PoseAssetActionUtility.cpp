#include "PoseAssetActionUtility.h"

#include "Animation/PoseAsset.h"
#include "Animation/AnimSequence.h"
#include "EditorUtilityLibrary.h"

void UPoseAssetActionsUtility::RefreshPoseAssetsFromSourceAnimation()
{
    TArray<UObject*> SelectedAssets =
        UEditorUtilityLibrary::GetSelectedAssets();

    int32 Updated = 0;
    int32 Skipped = 0;

    for (UObject* Obj : SelectedAssets)
    {
        UPoseAsset* PoseAsset = Cast<UPoseAsset>(Obj);

        if (!PoseAsset)
        {
            continue;
        }

        // Get source anim using reflected property lookup
        static FName SourceAnimName(TEXT("SourceAnimation"));

        FObjectProperty* SourceProperty =
            FindFProperty<FObjectProperty>(
                PoseAsset->GetClass(),
                SourceAnimName);

        if (!SourceProperty)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("%s: couldn't find SourceAnimation property"),
                *PoseAsset->GetName());

            Skipped++;
            continue;
        }

        UAnimSequence* SourceAnim =
            Cast<UAnimSequence>(
                SourceProperty->GetObjectPropertyValue_InContainer(
                    PoseAsset));

        if (!SourceAnim)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("%s: no source animation"),
                *PoseAsset->GetName());

            Skipped++;
            continue;
        }

        PoseAsset->Modify();

        PoseAsset->UpdatePoseFromAnimation(SourceAnim);

        PoseAsset->MarkPackageDirty();
        PoseAsset->PostEditChange();

        UE_LOG(LogTemp, Display,
            TEXT("Updated %s"),
            *PoseAsset->GetName());

        Updated++;
    }

    UE_LOG(LogTemp, Display,
        TEXT("Finished. Updated=%d Skipped=%d"),
        Updated,
        Skipped);
}