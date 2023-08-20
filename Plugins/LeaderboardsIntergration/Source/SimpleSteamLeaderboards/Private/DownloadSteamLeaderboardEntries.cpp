// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "DownloadSteamLeaderboardEntries.h"
#include "Engine.h"
#include <algorithm>
#include <string>

UDownloadSteamLeaderboardEntries* UDownloadSteamLeaderboardEntries::DownloadLeaderboardEntries(FString SteamLeaderboardName, ELeaderboardDataRequestUE4 DataRequestType, int32 MaxAmountOfEntries, int32 RangeStart, int32 RangeEnd)
{
	UDownloadSteamLeaderboardEntries* selfRef = NewObject<UDownloadSteamLeaderboardEntries>();

	selfRef->steamLeaderboardName = SteamLeaderboardName;
	selfRef->dataRequestType = DataRequestType;
	selfRef->maxAmountOfEntries = MaxAmountOfEntries;
	selfRef->rangeStart = RangeStart;
	selfRef->rangeEnd = RangeEnd;

	return selfRef;
}

void UDownloadSteamLeaderboardEntries::Activate()
{
	if (SteamAPI_Init())
	{
		FindSteamLeaderboard();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to initialize the Steam API!"));

		onFailure.Broadcast(DownloadedEntries);
		RemoveFromRoot();
	}
}

void UDownloadSteamLeaderboardEntries::FindSteamLeaderboard()
{
	const char* LeaderboardNameChar = TCHAR_TO_ANSI(*steamLeaderboardName);
	SteamAPICall_t FoundLeaderboard = SteamUserStats()->FindLeaderboard(LeaderboardNameChar);

	m_callResultFindLeaderboard.Set(FoundLeaderboard, this, &UDownloadSteamLeaderboardEntries::OnSteamLeaderboardFound);
}

void UDownloadSteamLeaderboardEntries::OnSteamLeaderboardFound(LeaderboardFindResult_t* pCallback, bool bIOFailure)
{
	if (!bIOFailure) 
	{
		foundSteamLeaderboard = pCallback->m_hSteamLeaderboard;

		if (foundSteamLeaderboard != 0) 
		{
			DownloadSteamLeaderboardEntries();
		} 
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to find table! (Table ID = 0)"));

			onFailure.Broadcast(DownloadedEntries);
			RemoveFromRoot();
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IO Failure! (Failed to find leaderboard)"));

		onFailure.Broadcast(DownloadedEntries);
		RemoveFromRoot();
	}
}

void UDownloadSteamLeaderboardEntries::DownloadSteamLeaderboardEntries()
{
	if (foundSteamLeaderboard != 0)
	{
		ELeaderboardDataRequest DataRequest;
		switch (dataRequestType)
		{
			case ELeaderboardDataRequestUE4::LDR_ELeaderboardDataRequestGlobal:
				DataRequest = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobal;
				break;

			case ELeaderboardDataRequestUE4::LDR_ELeaderboardDataRequestGlobalAroundUser:
				DataRequest = ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser;
				break;

			case ELeaderboardDataRequestUE4::LDR_ELeaderboardDataRequestFriends:
				DataRequest = ELeaderboardDataRequest::k_ELeaderboardDataRequestFriends;
				break;
		}

		SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(foundSteamLeaderboard, DataRequest, rangeStart, rangeEnd);
		m_callDownloadedSteamLeaderboardEntries.Set(hSteamAPICall, this, &UDownloadSteamLeaderboardEntries::OnDownloadedSteamLeaderboardEntries);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to find leaderboard! (Leaderboard ID = 0)"));

		onFailure.Broadcast(DownloadedEntries);
		RemoveFromRoot();
	}
}

void UDownloadSteamLeaderboardEntries::OnDownloadedSteamLeaderboardEntries(LeaderboardScoresDownloaded_t* pCallback, bool bIOFailure)
{
	if (!bIOFailure)
	{
		FOnlineSubsystemSteam* SteamSubsystem = static_cast<FOnlineSubsystemSteam*>(IOnlineSubsystem::Get());
		SteamLeaderboardEntries_t Entries = pCallback->m_hSteamLeaderboardEntries;

		maxAmountOfEntries = std::min(pCallback->m_cEntryCount, maxAmountOfEntries);

		SteamSubsystem->ExecuteNextTick([Entries, this] {
			LeaderboardEntry_t SteamLeaderboardEntry;

			for (int index = 0; index < maxAmountOfEntries; index++)
			{
				SteamUserStats()->GetDownloadedLeaderboardEntry(Entries, index, &SteamLeaderboardEntry, NULL, 0);

				FSteamLeaderboardEntry Entry;
				Entry.SetGlobalRank(SteamLeaderboardEntry.m_nGlobalRank);
				Entry.SetPlayerSteamID(ANSI_TO_TCHAR(std::to_string(SteamLeaderboardEntry.m_steamIDUser.ConvertToUint64()).c_str()));
				Entry.SetPlayerName(ANSI_TO_TCHAR(SteamFriends()->GetFriendPersonaName(SteamLeaderboardEntry.m_steamIDUser)));
				Entry.SetScore(SteamLeaderboardEntry.m_nScore);

				DownloadedEntries.Add(Entry);
			}

			onSuccess.Broadcast(DownloadedEntries);
		});
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IO Failure! (Failed to download leaderboard entries)"));

		onFailure.Broadcast(DownloadedEntries);
	}

	RemoveFromRoot();
}
