#include "Misc/NeonGameInstance.h"

#include "Online/OnlineServices.h"
#include "Online/Auth.h"
#include "Online/Lobbies.h"
#include "Online/Sessions.h"
#include "Online/OnlineResult.h"
#include "Online/OnlineAsyncOp.h"

#define GAME_SESSION_NAME TEXT("GameSession")
#define SESSIONATTRIBUTENAME_SESSIONNAME TEXT("ServerName")
#define SESSIONATTRIBUTENAME_CURRENTPLAYERCOUNT TEXT("CurrentPlayerCount")
#define SESSIONATTRIBUTENAME_HOSTNAME TEXT("HostName")
#define SESSIONATTRIBUTENAME_HASPASSWORD TEXT("HasPassword")

using namespace UE::Online;

void UNeonGameInstance::Init()
{
	Super::Init();

	Super::Init();

	auto Services = GetServices();
	if (!Services)
	{
		return;
	}

	auto Sessions = Services->GetSessionsInterface();
	if (!Sessions)
	{
		return;
	}

	SessionUpdatedHandle = Sessions->OnSessionUpdated().Add([this](const FSessionUpdated& Event) {HandleSessionUpdated(Event);});
	SessionCreatedHandle = Sessions->OnSessionCreated().Add([this](const FSessionCreated& Event) {OnSessionStateChanged.Broadcast();});
	SessionJoinedHandle = Sessions->OnSessionJoined().Add([this](const FSessionJoined& Event) {OnSessionStateChanged.Broadcast();});
	SessionLeftHandle = Sessions->OnSessionLeft().Add([this](const FSessionLeft& Event) {OnSessionStateChanged.Broadcast();});
}

void UNeonGameInstance::HandleSessionUpdated(const FSessionUpdated& Event)
{
	OnSessionStateChanged.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Session Updated: Added=%d Removed=%d"), Event.SessionUpdate.AddedSessionMembers.Num(), Event.SessionUpdate.RemovedSessionMembers.Num());

	if (Event.SessionName != GAME_SESSION_NAME)
	{
		return;
	}

	if (Event.SessionUpdate.AddedSessionMembers.Num() == 0 && Event.SessionUpdate.RemovedSessionMembers.Num() == 0)
	{
		return;
	}

	auto Services = GetServices();
	if (!Services)
	{
		return;
	}

	auto Sessions = Services->GetSessionsInterface();
	if (!Sessions)
	{
		return;
	}

	FGetSessionByName::Params GetParams;
	GetParams.LocalName = GAME_SESSION_NAME;

	auto SessionResult = Sessions->GetSessionByName(MoveTemp(GetParams));

	if (SessionResult.IsError())
	{
		return;
	}

	const auto& Session = SessionResult.GetOkValue().Session;

	if (Session->GetOwnerAccountId() != LocalHostAccountId)
	{
		return;
	}

	const FSessionSettings& Settings = Session->GetSessionSettings();

	const int32 CurrentPlayers = Settings.NumMaxConnections - Session->GetNumOpenConnections();

	FUpdateSessionSettings::Params UpdateParams;
	UpdateParams.LocalAccountId = LocalHostAccountId;
	UpdateParams.SessionName = GAME_SESSION_NAME;

	FCustomSessionSetting PlayerCountSetting;
	PlayerCountSetting.Data = FSchemaVariant((int64)CurrentPlayers);
	PlayerCountSetting.Visibility = ESchemaAttributeVisibility::Public;
	PlayerCountSetting.ID = 0;

	UpdateParams.Mutations.UpdatedCustomSettings.Add(FSchemaAttributeId(SESSIONATTRIBUTENAME_CURRENTPLAYERCOUNT), MoveTemp(PlayerCountSetting));

	auto UpdateOp = Sessions->UpdateSessionSettings(MoveTemp(UpdateParams));

	UpdateOp.OnComplete([](const TOnlineResult<FUpdateSessionSettings>& Result)
		{
			if (Result.IsError())
			{
				UE_LOG(LogTemp, Error, TEXT("UpdateSessionSettings Failed: %s"), *Result.GetErrorValue().GetLogString());
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Session Player Count Updated"));
		});
}

void UNeonGameInstance::Shutdown()
{
	auto Services = GetServices();

	if (Services)
	{
		auto Auth = Services->GetAuthInterface();
		auto Sessions = Services->GetSessionsInterface();

		if (Auth && Sessions)
		{
			auto UsersResult =
				Auth->GetAllLocalOnlineUsers({});

			if (!UsersResult.IsError() && UsersResult.GetOkValue().AccountInfo.Num() > 0)
			{
				const FAccountId AccountId = UsersResult.GetOkValue().AccountInfo[0]->AccountId;
				UE_LOG(LogTemp, Warning, TEXT("Account Service Type: %d"), (int32)AccountId.GetOnlineServicesType());

				FLeaveSession::Params Params;
				Params.LocalAccountId = AccountId;
				Params.SessionName = GAME_SESSION_NAME;
				Params.bDestroySession = true;

				Sessions->LeaveSession(MoveTemp(Params));
			}
		}
	}


	Super::Shutdown();
}

bool UNeonGameInstance::TestOSSSv2()
{
	IOnlineServicesPtr Services = GetServices();

	if (!Services)
	{
		UE_LOG(LogTemp, Error, TEXT("No Online Services"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Online Services Provider found.")
	);

	return true;
}

bool UNeonGameInstance::TestAuth()
{
	auto Services = GetServices();

	if (!Services)
	{
		UE_LOG(LogTemp, Error, TEXT("No Services"));
		return false;
	}

	auto Auth = Services->GetAuthInterface();

	if (!Auth)
	{
		UE_LOG(LogTemp, Error, TEXT("No Auth Interface"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Auth Interface Found"));
	return true;
}

void UNeonGameInstance::LogInToEOS()
{
	IOnlineServicesPtr Services = GetServices();

	if (!Services)
	{
		UE_LOG(LogTemp, Error, TEXT("No Online Services"));
		return;
	}

	IAuthPtr Auth = Services->GetAuthInterface();

	if (!Auth)
	{
		UE_LOG(LogTemp, Error, TEXT("No Auth Interface"));
		return;
	}

	FAuthLogin::Params Params;

	// Local player 0
	Params.PlatformUserId = FPlatformUserId::CreateFromInternalId(0);

	// Epic Account Portal login
	Params.CredentialsType = TEXT("accountportal");

	auto LoginOp = Auth->Login(MoveTemp(Params));

	LoginOp.OnComplete(
		[](const TOnlineResult<FAuthLogin>& Result)
		{
			if (Result.IsError())
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("EOS Login Failed: %s"),
					*Result.GetErrorValue().GetLogString()
				);

				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("EOS Login Successful"));
		});
}

bool UNeonGameInstance::IsPlayerLoggedIn()
{
	using namespace UE::Online;

	IOnlineServicesPtr Services = GetServices();

	if (!Services)
	{
		return false;
	}

	IAuthPtr Auth = Services->GetAuthInterface();

	if (!Auth)
	{
		return false;
	}

	FPlatformUserId PlatformUserId = FPlatformUserId::CreateFromInternalId(0);

	return Auth->IsLoggedIn(PlatformUserId);
}

void UNeonGameInstance::CreateOnlineSession(APlayerController* PlayerController, const FString& ServerName, const FString& Password, int32 MaxPlayers)
{
	using namespace UE::Online;

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerController"));
		return;
	}

	auto Services = GetServices();

	if (!Services)
	{
		UE_LOG(LogTemp, Error, TEXT("No Online Services"));
		return;
	}

	auto Auth = Services->GetAuthInterface();
	auto Sessions = Services->GetSessionsInterface();

	if (!Auth || !Sessions)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Auth or Sessions"));
		return;
	}


	//If needed become listen server first
	if (UWorld* World = GetWorld())
	{
		if (World->GetNetMode() != NM_ListenServer)
		{
			const FString MapPath =
				World->GetOutermost()->GetName();

			World->ServerTravel(MapPath + TEXT("?listen"));

			return;
		}
	}

	const FPlatformUserId PlatformUserId = PlayerController->GetPlatformUserId();

	auto UserResult = Auth->GetLocalOnlineUserByPlatformUserId({ PlatformUserId });
	if (UserResult.IsError())
	{
		UE_LOG(LogTemp, Error, TEXT("Player is not logged in"));
		return;
	}

	LocalHostAccountId = UserResult.GetOkValue().AccountInfo->AccountId;

	FCreateSession::Params Params;

	Params.LocalAccountId = LocalHostAccountId;
	Params.SessionName = GAME_SESSION_NAME;

	Params.bPresenceEnabled = true;
	Params.bIsLANSession = false;
	Params.bAllowSanctionedPlayers = true;

	Params.SessionSettings.SchemaName = GAME_SESSION_NAME;
	Params.SessionSettings.NumMaxConnections = MaxPlayers;
	Params.SessionSettings.JoinPolicy = ESessionJoinPolicy::Public;
	Params.SessionSettings.bAllowNewMembers = true;

	FCustomSessionSetting HostNameSettings;
	HostNameSettings.Data = GetPlayerDisplayName(PlayerController);
	HostNameSettings.Visibility = ESchemaAttributeVisibility::Public;
	HostNameSettings.ID = 0;

	Params.SessionSettings.CustomSettings.Add(
		FSchemaAttributeId(SESSIONATTRIBUTENAME_HOSTNAME),
		HostNameSettings
	);

	FCustomSessionSetting CurrentPlayerCount;
	CurrentPlayerCount.Data = FSchemaVariant((int64)1);
	CurrentPlayerCount.Visibility = ESchemaAttributeVisibility::Public;
	CurrentPlayerCount.ID = 0;

	Params.SessionSettings.CustomSettings.Add(
		FSchemaAttributeId(SESSIONATTRIBUTENAME_CURRENTPLAYERCOUNT),
		CurrentPlayerCount
	);

	// Server Name
	FCustomSessionSetting ServerNameSetting;
	ServerNameSetting.Data = ServerName;
	ServerNameSetting.Visibility = ESchemaAttributeVisibility::Public;
	ServerNameSetting.ID = 0;

	Params.SessionSettings.CustomSettings.Add(
		FSchemaAttributeId(SESSIONATTRIBUTENAME_SESSIONNAME),
		ServerNameSetting
	);

	// Password Flag (don't store actual password)
	FCustomSessionSetting PasswordSetting;
	PasswordSetting.Data = !Password.IsEmpty();
	PasswordSetting.Visibility = ESchemaAttributeVisibility::Public;
	PasswordSetting.ID = 1;

	Params.SessionSettings.CustomSettings.Add(
		FSchemaAttributeId(SESSIONATTRIBUTENAME_HASPASSWORD),
		MoveTemp(PasswordSetting)
	);

	auto CreateOp = Sessions->CreateSession(MoveTemp(Params));

	CreateOp.OnComplete(
		[this, Password, Sessions](const TOnlineResult<FCreateSession>& Result)
		{
			if (Result.IsError())
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Create Session Failed: %s"),
					*Result.GetErrorValue().GetLogString()
				);
				return;
			}

			// Store the actual password locally only.
			CurrentSessionPassword = Password;

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Session Created Successfully")
			);

			OnSessionStateChanged.Broadcast();
		});
}

FString UNeonGameInstance::GetPlayerDisplayName(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return FString();
	}

	auto Services = GetServices();

	if (!Services)
	{
		return FString();
	}

	auto Auth = Services->GetAuthInterface();

	if (!Auth)
	{
		return FString();
	}

	const FPlatformUserId PlatformUserId = PlayerController->GetPlatformUserId();

	auto UserResult = Auth->GetLocalOnlineUserByPlatformUserId({ PlatformUserId });
	if (UserResult.IsError())
	{
		return FString();
	}

	const TSharedRef<FAccountInfo>& AccountInfo = UserResult.GetOkValue().AccountInfo;

	const FSchemaVariant* DisplayName = AccountInfo->Attributes.Find(FSchemaAttributeId(TEXT("DisplayName"))
	);

	if (!DisplayName)
	{
		return FString();
	}

	return DisplayName->GetString();
}

bool UNeonGameInstance::IsPlayerInSession(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return false;
	}

	auto Services = GetServices();

	if (!Services)
	{
		return false;
	}

	auto Sessions = Services->GetSessionsInterface();

	if (!Sessions)
	{
		return false;
	}

	const FPlatformUserId PlatformUserId = PlayerController->GetPlatformUserId();

	auto Auth = Services->GetAuthInterface();

	if (!Auth)
	{
		return false;
	}

	auto UserResult = Auth->GetLocalOnlineUserByPlatformUserId({ PlatformUserId });

	if (UserResult.IsError())
	{
		return false;
	}

	const FAccountId AccountId = UserResult.GetOkValue().AccountInfo->AccountId;
	UE_LOG(LogTemp, Warning, TEXT("Account Service Type: %d"), (int32)AccountId.GetOnlineServicesType());

	FGetSessionByName::Params Params;
	Params.LocalName = GAME_SESSION_NAME;

	auto SessionResult = Sessions->GetSessionByName(MoveTemp(Params));

	return !SessionResult.IsError();
}

void UNeonGameInstance::FindAvailableSessions(APlayerController* PlayerController, const FString& SessionName)
{
	using namespace UE::Online;

	FoundSessions.Empty();

	auto Services = GetServices();

	if (!Services)
	{
		return;
	}

	auto Auth = Services->GetAuthInterface();
	auto Sessions = Services->GetSessionsInterface();

	if (!Auth || !Sessions)
	{
		return;
	}

	const FPlatformUserId PlatformUserId = PlayerController->GetPlatformUserId();

	auto UserResult = Auth->GetLocalOnlineUserByPlatformUserId({ PlatformUserId });

	if (UserResult.IsError())
	{
		return;
	}

	const FAccountId AccountId = UserResult.GetOkValue().AccountInfo->AccountId;
	UE_LOG(LogTemp, Warning, TEXT("Account Service Type: %d"), (int32)AccountId.GetOnlineServicesType());

	FFindSessions::Params Params;
	Params.LocalAccountId = AccountId;
	Params.MaxResults = 100;

	FFindSessionsSearchFilter Filter;
	Filter.Key = FSchemaAttributeId(SESSIONATTRIBUTENAME_SESSIONNAME);
	Filter.Value = SessionName;
	Filter.ComparisonOp = ESchemaAttributeComparisonOp::Equals;

	Params.Filters.Add(Filter);

	auto FindOp = Sessions->FindSessions(MoveTemp(Params));

	FindOp.OnComplete(
		[this, Sessions](const TOnlineResult<FFindSessions>& Result)
		{
			if (Result.IsError())
			{
				UE_LOG(LogTemp, Error,
					TEXT("FindSessions Failed"));
				PostFindSessions.Broadcast(false);
				PostFindSessions.Clear();
				return;
			}

			for (const auto& SessionId :
				Result.GetOkValue().FoundSessionIds)
			{
				FGetSessionById::Params GetParams;
				GetParams.SessionId = SessionId;

				auto SessionResult = Sessions->GetSessionById(MoveTemp(GetParams));

				if (SessionResult.IsError())
				{
					continue;
				}

				const auto& Session = SessionResult.GetOkValue().Session;

				const FSessionSettings& Settings = Session->GetSessionSettings();
				FFoundSession Found;
				Found.SessionId = Session->GetSessionId();
				Found.OpenSlots = Session->GetNumOpenConnections();

				if (const auto* ServerName = Settings.CustomSettings.Find(FSchemaAttributeId(SESSIONATTRIBUTENAME_SESSIONNAME)))
				{
					Found.ServerName = ServerName->Data.GetString();
				}

				if (const auto* HasPassword = Settings.CustomSettings.Find(
					FSchemaAttributeId(SESSIONATTRIBUTENAME_HASPASSWORD)))
				{
					Found.bHasPassword = HasPassword->Data.GetBoolean();
				}

				if (const auto* HostName = Settings.CustomSettings.Find(
					FSchemaAttributeId(SESSIONATTRIBUTENAME_HOSTNAME)))
				{
					Found.HostName = HostName->Data.GetString();
				}

				FoundSessions.Add(Found);
			}

			PostFindSessions.Broadcast(true);
			PostFindSessions.Clear();

			UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), FoundSessions.Num());
		});
}

void UNeonGameInstance::JoinOnlineSession(APlayerController* PlayerController, int32 SessionIndex)
{
	if (!ensure(PlayerController))
	{
		return;
	}

	if (!FoundSessions.IsValidIndex(SessionIndex))
	{
		return;
	}

	auto Services = GetServices();

	if (!Services)
	{
		return;
	}

	auto Auth = Services->GetAuthInterface();
	auto Sessions = Services->GetSessionsInterface();

	if (!Auth || !Sessions)
	{
		return;
	}

	const FPlatformUserId PlatformUserId = PlayerController->GetPlatformUserId();

	auto UserResult = Auth->GetLocalOnlineUserByPlatformUserId({ PlatformUserId });

	if (UserResult.IsError())
	{
		return;
	}

	const FAccountId AccountId = UserResult.GetOkValue().AccountInfo->AccountId;
	UE_LOG(LogTemp, Warning, TEXT("Account Service Type: %d"), (int32)AccountId.GetOnlineServicesType());

	FJoinSession::Params Params;

	Params.LocalAccountId = AccountId;
	Params.SessionName = GAME_SESSION_NAME;
	Params.SessionId = FoundSessions[SessionIndex].SessionId;

	Params.bPresenceEnabled = true;

	auto JoinOp = Sessions->JoinSession(MoveTemp(Params));

	JoinOp.OnComplete(
		[this, Services, Sessions, AccountId, SessionIndex](const TOnlineResult<FJoinSession>& Result)
		{
			if (Result.IsError())
			{
				UE_LOG(LogTemp, Error, TEXT("Join Session Failed"));
				return;
			}

			auto SessionResult = Sessions->GetSessionByName({ TEXT("GameSession") });

			FString ConnectString;

			if (SessionResult.IsOk())
			{
				const auto& Session =
					SessionResult.GetOkValue().Session;

				const FSessionSettings& Settings =
					Session->GetSessionSettings();

				const FCustomSessionSetting* ConnectSetting =
					Settings.CustomSettings.Find(
						FSchemaAttributeId(TEXT("CONNECT_STRING")));

				if (!ConnectSetting)
				{
					UE_LOG(LogTemp, Error,
						TEXT("Session has no CONNECT_STRING"));

					return;
				}

				ConnectString = ConnectSetting->Data.GetString();
			}
			else
			{
				UE_LOG(LogTemp, Error,
					TEXT("Failed to retrieve joined session"));

				return;
			}

			UE_LOG(LogTemp, Warning,
				TEXT("Travelling to %s"),
				*ConnectString);

			if (APlayerController* PC = GetFirstLocalPlayerController())
			{
				PC->ClientTravel(
					ConnectString,
					TRAVEL_Absolute);
			}
		});
}

FSessionInfoData UNeonGameInstance::GetCurrentSessionInfo()
{
	using namespace UE::Online;

	FSessionInfoData Result;

	auto Services = GetServices();
	if (!Services)
	{
		return Result;
	}

	auto Sessions = Services->GetSessionsInterface();
	if (!Sessions)
	{
		return Result;
	}

	FGetSessionByName::Params Params;
	Params.LocalName = GAME_SESSION_NAME;

	auto SessionResult = Sessions->GetSessionByName(MoveTemp(Params));
	if (SessionResult.IsError())
	{
		return Result;
	}

	const TSharedRef<const ISession>& Session = SessionResult.GetOkValue().Session;

	Result.bValid = true;
	Result.OpenSlots = Session->GetNumOpenConnections();

	const FSessionSettings& Settings = Session->GetSessionSettings();

	if (const auto* ServerName = Settings.CustomSettings.Find(FSchemaAttributeId(SESSIONATTRIBUTENAME_SESSIONNAME)))
	{
		Result.ServerName = ServerName->Data.GetString();
	}

	if (const auto* HostName = Settings.CustomSettings.Find(FSchemaAttributeId(SESSIONATTRIBUTENAME_HOSTNAME)))
	{
		Result.HostName = HostName->Data.GetString();
	}

	if (const auto* Password = Settings.CustomSettings.Find(FSchemaAttributeId(SESSIONATTRIBUTENAME_HASPASSWORD)))
	{
		Result.bHasPassword = Password->Data.GetBoolean();
	}

	if (const auto* PlayerCount = Settings.CustomSettings.Find(FSchemaAttributeId(SESSIONATTRIBUTENAME_CURRENTPLAYERCOUNT)))
	{
		Result.CurrentPlayerCount = PlayerCount->Data.GetInt64();
	}


	return Result;
}

void UNeonGameInstance::LeaveCurrentSession(APlayerController* PlayerController)
{
	using namespace UE::Online;

	auto Services = GetServices();
	if (!Services)
	{
		return;
	}

	auto Auth = Services->GetAuthInterface();
	auto Sessions = Services->GetSessionsInterface();

	if (!Auth || !Sessions || !PlayerController)
	{
		return;
	}

	FAuthGetLocalOnlineUserByPlatformUserId::Params UserParams;
	UserParams.PlatformUserId = PlayerController->GetPlatformUserId();

	auto UserResult = Auth->GetLocalOnlineUserByPlatformUserId(MoveTemp(UserParams));

	if (UserResult.IsError())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get local account"));
		return;
	}

	FLeaveSession::Params Params;
	Params.LocalAccountId = UserResult.GetOkValue().AccountInfo->AccountId;

	Params.SessionName = GAME_SESSION_NAME;

	// If we're the host, destroy the session.
	// If we're a client, just leave it.
	Params.bDestroySession = false;

	auto SessionResult = Sessions->GetSessionByName(FGetSessionByName::Params{ GAME_SESSION_NAME });

	if (!SessionResult.IsError())
	{
		const auto& Session = SessionResult.GetOkValue().Session;

		if (Session->GetOwnerAccountId() == Params.LocalAccountId)
		{
			Params.bDestroySession = true;
		}
	}

	auto LeaveOp = Sessions->LeaveSession(MoveTemp(Params));

	LeaveOp.OnComplete(
		[this](const TOnlineResult<FLeaveSession>& Result)
		{
			if (Result.IsError())
			{
				UE_LOG(LogTemp, Error, TEXT("Leave Session Failed: %s"), *Result.GetErrorValue().GetLogString());

				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Left Session Successfully"));

			OnSessionStateChanged.Broadcast();
		});
}