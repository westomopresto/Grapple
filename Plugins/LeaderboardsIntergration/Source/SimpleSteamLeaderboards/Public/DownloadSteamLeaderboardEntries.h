// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4265)
#pragma warning(disable:4701)

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemSteam.h"
#include <steam/steam_api.h>
#include "DownloadSteamLeaderboardEntries.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class ELeaderboardDataRequestUE4 : uint8
{
	LDR_ELeaderboardDataRequestGlobal                UMETA(DisplayName = "Request Global"),
	LDR_ELeaderboardDataRequestGlobalAroundUser      UMETA(DisplayName = "Request Global Around User"),
	LDR_ELeaderboardDataRequestFriends               UMETA(DisplayName = "Request Friends"),
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FSteamLeaderboardEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Simple Steam Leaderboards")
		int32 GlobalRank;
	UPROPERTY(BlueprintReadOnly, Category = "Simple Steam Leaderboards")
		FString PlayerSteamID;
	UPROPERTY(BlueprintReadOnly, Category = "Simple Steam Leaderboards")
		FString PlayerName;
	UPROPERTY(BlueprintReadOnly, Category = "Simple Steam Leaderboards")
		int32 Score;

	// Set
	void SetGlobalRank(const int32 NewGlobalRank) { GlobalRank = NewGlobalRank; }
	void SetPlayerSteamID(const FString NewPlayerSteamID) { PlayerSteamID = NewPlayerSteamID; }
	void SetPlayerName(const FString NewPlayerName) { PlayerName = NewPlayerName; }
	void SetScore(const int32 NewScore) { Score = NewScore; }

	// Get
	int32 GetGlobalRank() { return GlobalRank; }
	FString GetPlayerSteamID() { return PlayerSteamID; }
	FString GetPlayerName() { return PlayerName; }
	int32 GetScore() { return Score; }

	// Constructor
	FSteamLeaderboardEntry()
	{
		GlobalRank = 0;
		PlayerSteamID = "";
		PlayerName = "";
		Score = 0;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseDelegatge, const TArray<FSteamLeaderboardEntry>&, DownloadedEntries);

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class SIMPLESTEAMLEADERBOARDS_API UDownloadSteamLeaderboardEntries : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FResponseDelegatge onSuccess;

	UPROPERTY(BlueprintAssignable)
		FResponseDelegatge onFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Simple Steam Leaderboards")
		static UDownloadSteamLeaderboardEntries* DownloadLeaderboardEntries(FString SteamLeaderboardName, ELeaderboardDataRequestUE4 DataRequestType, int32 MaxAmountOfEntries = 10, int32 RangeStart = -5, int32 RangeEnd = 4);


	UPROPERTY()
		FString steamLeaderboardName;

	UPROPERTY()
		ELeaderboardDataRequestUE4 dataRequestType;

	UPROPERTY()
		int32 maxAmountOfEntries;

	UPROPERTY()
		int32 rangeStart;

	UPROPERTY()
		int32 rangeEnd;
private:
	virtual void Activate() override;

	void FindSteamLeaderboard();
	void OnSteamLeaderboardFound(LeaderboardFindResult_t* pCallback, bool bIOFailure);
	CCallResult<UDownloadSteamLeaderboardEntries, LeaderboardFindResult_t> m_callResultFindLeaderboard;

	SteamLeaderboard_t foundSteamLeaderboard;

	void DownloadSteamLeaderboardEntries();
	void OnDownloadedSteamLeaderboardEntries(LeaderboardScoresDownloaded_t* pCallback, bool bIOFailure);
	CCallResult<UDownloadSteamLeaderboardEntries, LeaderboardScoresDownloaded_t> m_callDownloadedSteamLeaderboardEntries;

	TArray<FSteamLeaderboardEntry> DownloadedEntries;
};
