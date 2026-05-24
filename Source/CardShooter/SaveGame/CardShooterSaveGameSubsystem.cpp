#include "CardShooterSaveGameSubsystem.h"

#include "Misc/Paths.h"
#include <Kismet/GameplayStatics.h>
#include "CardShooterSaveData.h"

#define PLAYER_SAVEDATA_PREFIX "PlayerData_"
#define PLAYER_SAVEDATA_DEFAULTPROFILE "PlayerData_Default"


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
            if (auto* AsCardShooterSaveData = Cast<UCardShooterSaveData>(UGameplayStatics::LoadGameFromSlot(FPaths::GetBaseFilename(File), 0)))
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
 
void UCardShooterSaveGameSubsystem::GetLocalPlayerSaveGameName(TArray<FString>& OutSaveGames)
{
    OutSaveGames.Empty();

    TArray<FString> Files;

    // Get files only (no directories)
    IFileManager::Get().FindFiles(Files, *GetPlayerSaveGameDir(), TEXT("sav"));


    for (const FString& File : Files)
    {
        if (File.StartsWith(PLAYER_SAVEDATA_PREFIX))
        {
            //We'll just assume this is a UCardShooterSaveData
            OutSaveGames.Add(FPaths::GetBaseFilename(File));
        }
    }

    OutSaveGames.Sort([this](const FString& A, const FString& B)
        {
            const FString Dir = GetPlayerSaveGameDir();

            const FDateTime TimeA = IFileManager::Get().GetTimeStamp(*(Dir / A + ".sav"));
            const FDateTime TimeB = IFileManager::Get().GetTimeStamp(*(Dir / B + ".sav"));

            return TimeA > TimeB; // newer first
        });
}

bool UCardShooterSaveGameSubsystem::SaveGame(bool bIsAutoSave)
{
    if (!LocalPlayerSaveData)
    {
        return false;
    }
    LocalPlayerSaveData->BeforeSave();
    return UGameplayStatics::SaveGameToSlot(LocalPlayerSaveData, PLAYER_SAVEDATA_PREFIX + LocalPlayerSaveData->ProfileName, 0);
}

FString UCardShooterSaveGameSubsystem::GetPlayerSaveGameDir()
{
    FString SaveGamesPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/");

    return SaveGamesPath;
}

void UCardShooterSaveGameSubsystem::SetPlayerLoadedSaveData(UCardShooterSaveData* Data)
{
    LocalPlayerSaveData = Data;
}

UCardShooterSaveData* UCardShooterSaveGameSubsystem::GetLocalPlayerSaveData()
{
#if WITH_EDITOR
    if (!LocalPlayerSaveData)
    {
        if (UCardShooterSaveData* LoadedSave = Cast<UCardShooterSaveData>(UGameplayStatics::LoadGameFromSlot(PLAYER_SAVEDATA_DEFAULTPROFILE , 0)))
        {
            LocalPlayerSaveData = LoadedSave;
            LocalPlayerSaveData->AfterLoad();
        }
        else
        {
            UCardShooterSaveData* NewData = NewObject<UCardShooterSaveData>(GetLocalPlayer());
            NewData->ProfileName = TEXT("Default");
            LocalPlayerSaveData = NewData;
            SaveGame();
        }
    }
#endif

    return LocalPlayerSaveData;
}

bool UCardShooterSaveGameSubsystem::GetLastEditedSave(FString& OutProfileName)
{
    TArray<FString> Saves;

    GetLocalPlayerSaveGameName(Saves);

    if (Saves.IsEmpty())
    {
        return false;
    }

    OutProfileName = Saves[0];

    return true;
}

