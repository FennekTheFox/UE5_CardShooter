#include "CardShooterSaveGameSubsystem.h"

#include "Misc/Paths.h"
#include <Kismet/GameplayStatics.h>
#include "CardShooterSaveData.h"

#define PLAYER_SAVEDATA_PREFIX "PlayerData_"

void UCardShooterSaveGameSubsystem::GetLocalSaveGameList(TArray<UCardShooterSaveData*>& OutSaveGames)
{
    OutSaveGames.Empty();

    TArray<FString> Files;

    // Get files only (no directories)
    IFileManager::Get().FindFiles(Files, *GetPlayerSaveGameDir(), true, false);

    for (const FString& File : Files)
    {
        if (File.StartsWith(PLAYER_SAVEDATA_PREFIX))
        {
            if (auto* AsCardShooterSaveData = Cast<UCardShooterSaveData>(UGameplayStatics::LoadGameFromSlot(File, 0)))
            {
                OutSaveGames.Add(AsCardShooterSaveData);
            }
        }
    }

    OutSaveGames.Sort([]( UCardShooterSaveData& A, UCardShooterSaveData& B)
        {
            return A.DateTimeLastSaved.ToUnixTimestampDecimal() >=  B.DateTimeLastSaved.ToUnixTimestampDecimal();
        });
}
 
bool UCardShooterSaveGameSubsystem::SaveGame(UCardShooterSaveData* Data)
{
    return UGameplayStatics::SaveGameToSlot(Data, PLAYER_SAVEDATA_PREFIX + Data->SaveGameName, 0);
}

FString UCardShooterSaveGameSubsystem::GetPlayerSaveGameDir()
{
    return FPaths::ProjectSavedDir();
}

