// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/Sessions.h"

#include "NeonGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FFoundSession
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString ServerName = "N/A";

	UPROPERTY(BlueprintReadOnly)
	FString HostName = "N/A";

	UPROPERTY(BlueprintReadOnly)
	bool bHasPassword = false;

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerCount = -1;

	UPROPERTY(BlueprintReadOnly)
	int32 OpenSlots = -1;

	UE::Online::FOnlineSessionId SessionId;
};

USTRUCT(BlueprintType)
struct FSessionInfoData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bValid = false;

	UPROPERTY(BlueprintReadOnly)
	FString ServerName = "N/A";

	UPROPERTY(BlueprintReadOnly)
	FString HostName = "N/A";

	UPROPERTY(BlueprintReadOnly)
	bool bHasPassword = false;

	UPROPERTY(BlueprintReadOnly)
	FString ServerPassword;


	UPROPERTY(BlueprintReadOnly)
	int32 OpenSlots = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayerCount = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> MemberAccountIds;

	//UPROPERTY(BlueprintReadOnly)
	//FString SessionId;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionStateChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostFindSessions, bool, bSuccess);

UCLASS()
class UNeonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;
	void Shutdown() override;


	UFUNCTION(BlueprintCallable)
	bool TestOSSSv2();

	UFUNCTION(BlueprintCallable)
	bool TestAuth();

	UFUNCTION(BlueprintCallable)
	void LogInToEOS();

	UFUNCTION(BlueprintPure)
	bool IsPlayerLoggedIn();

	UFUNCTION(BlueprintCallable)
	void CreateOnlineSession(APlayerController* PlayerController, const FString& ServerName, const FString& Password, int32 MaxPlayers);

	UFUNCTION(BlueprintPure)
	FString GetPlayerDisplayName(APlayerController* PlayerController);

	UFUNCTION(BlueprintPure)
	bool IsPlayerInSession(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void FindAvailableSessions(APlayerController* PlayerController, const FString& SessionName);

	UFUNCTION(BlueprintCallable)
	void JoinOnlineSession(APlayerController* PlayerController,int32 SessionIndex);
	
	UFUNCTION(BlueprintCallable)
	void LeaveCurrentSession(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	FSessionInfoData GetCurrentSessionInfo();


	UPROPERTY(BlueprintAssignable)
	FPostFindSessions PostFindSessions;

	UPROPERTY(BlueprintAssignable)
	FOnSessionStateChanged OnSessionStateChanged;

	UPROPERTY(BlueprintReadOnly)
	TArray<FFoundSession> FoundSessions;

private:
	void HandleSessionUpdated(const UE::Online::FSessionUpdated& Event);
	void UpdateCurrentPlayerCount(int32 Delta);
	bool IsSessionHost();
	void HandleSessionJoined(const UE::Online::FSessionJoined& Event);
	void HandleSessionLeft(const UE::Online::FSessionLeft& Event);

	FString CurrentSessionPassword;
	UE::Online::FAccountId LocalHostAccountId;

	UE::Online::FOnlineEventDelegateHandle  SessionCreatedHandle;
	UE::Online::FOnlineEventDelegateHandle  SessionJoinedHandle;
	UE::Online::FOnlineEventDelegateHandle  SessionLeftHandle;
	UE::Online::FOnlineEventDelegateHandle  SessionUpdatedHandle;
};
