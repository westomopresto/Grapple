// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "UploadScoreToSteamLeaderboard.h"
#include "Engine.h"

UUploadScoreToSteamLeaderboard* UUploadScoreToSteamLeaderboard::UploadScoreToSteamLeaderboard(FString SteamLeaderboardName, int32 Score, ELeaderboardUploadScoreMethodUE4 UploadMethod, EEasyLeaderboardUploadScoreMethod EasyUploadScoreMethod)
{
	UUploadScoreToSteamLeaderboard* selfRef = NewObject<UUploadScoreToSteamLeaderboard>();

	selfRef->steamLeaderboardName = SteamLeaderboardName;
	selfRef->score = Score;
	selfRef->uploadMethod = UploadMethod;
	selfRef->easyUploadScoreMethod = EasyUploadScoreMethod;

	return selfRef;
}

void UUploadScoreToSteamLeaderboard::Activate()
{
	if (SteamAPI_Init())
	{
		FindSteamLeaderboard();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to initialize the Steam API!"));

		onFailure.Broadcast(NULL);
		RemoveFromRoot();
	}
}

void UUploadScoreToSteamLeaderboard::FindSteamLeaderboard()
{
	const char* LeaderboardNameChar = TCHAR_TO_ANSI(*steamLeaderboardName);
	SteamAPICall_t FoundLeaderboard = SteamUserStats()->FindLeaderboard(LeaderboardNameChar);

	m_callResultFindLeaderboard.Set(FoundLeaderboard, this, &UUploadScoreToSteamLeaderboard::OnSteamLeaderboardFound);
}
void UUploadScoreToSteamLeaderboard::OnSteamLeaderboardFound(LeaderboardFindResult_t* pCallback, bool bIOFailure)
{
	if (!bIOFailure)
	{
		foundSteamLeaderboard = pCallback->m_hSteamLeaderboard;

		if (foundSteamLeaderboard != 0)
		{
			switch (easyUploadScoreMethod) 
			{
				case EEasyLeaderboardUploadScoreMethod::ELUSM_EEasyLeaderboardUploadScoreMethodSubtract:
				case EEasyLeaderboardUploadScoreMethod::ELUSM_EEasyLeaderboardUploadScoreMethodAdd:
					DownloadSteamLeaderboardPersonalScore();
					break;

				case EEasyLeaderboardUploadScoreMethod::ELUSM_EEasyLeaderboardUploadScoreMethodNone:
				default:
					UploadScoreToSteamLeaderboard();
					break;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to find leaderboard! (Leaderboard ID = 0)"));

			onFailure.Broadcast(NULL);
			RemoveFromRoot();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IO Failure! (Failed to find leaderboard)"));

		onFailure.Broadcast(NULL);
		RemoveFromRoot();
	}
}

void UUploadScoreToSteamLeaderboard::DownloadSteamLeaderboardPersonalScore()
{
	if (foundSteamLeaderboard != 0)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(foundSteamLeaderboard, ELeaderboardDataRequest::k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
		m_callDownloadedSteamLeaderboardEntries.Set(hSteamAPICall, this, &UUploadScoreToSteamLeaderboard::OnDownloadedSteamLeaderboardPersonalScore);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to find leaderboard! (Leaderboard ID = 0)"));

		onFailure.Broadcast(NULL);
		RemoveFromRoot();
	}
}
void UUploadScoreToSteamLeaderboard::OnDownloadedSteamLeaderboardPersonalScore(LeaderboardScoresDownloaded_t* pCallback, bool bIOFailure)
{
	if (!bIOFailure)
	{
		LeaderboardEntry_t SteamLeaderboardEntry;
		SteamUserStats()->GetDownloadedLeaderboardEntry(pCallback->m_hSteamLeaderboardEntries, 0, &SteamLeaderboardEntry, NULL, 0);

		switch (easyUploadScoreMethod)
		{
			case EEasyLeaderboardUploadScoreMethod::ELUSM_EEasyLeaderboardUploadScoreMethodSubtract:
				score = SteamLeaderboardEntry.m_nScore - score;
				break;

			case EEasyLeaderboardUploadScoreMethod::ELUSM_EEasyLeaderboardUploadScoreMethodAdd:
				score += SteamLeaderboardEntry.m_nScore;
				break;
		}

		UploadScoreToSteamLeaderboard();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IO Failure! (Failed to download leaderboard entries)"));

		onFailure.Broadcast(NULL);
		RemoveFromRoot();
	}
}

void UUploadScoreToSteamLeaderboard::UploadScoreToSteamLeaderboard()
{
	ELeaderboardUploadScoreMethod ConvertedUploadMethod;

	switch (uploadMethod)
	{
		case ELeaderboardUploadScoreMethodUE4::LUSM_ELeaderboardUploadScoreMethodForceUpdate:
			ConvertedUploadMethod = ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodForceUpdate;
			break;

		case ELeaderboardUploadScoreMethodUE4::LUSM_ELeaderboardUploadScoreMethodKeepBest:
			ConvertedUploadMethod = ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodKeepBest;
			break;

		case ELeaderboardUploadScoreMethodUE4::LUSM_ELeaderboardUploadScoreMethodNone:
			ConvertedUploadMethod = ELeaderboardUploadScoreMethod::k_ELeaderboardUploadScoreMethodNone;
			break;
	}

	SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(foundSteamLeaderboard, ConvertedUploadMethod, score, NULL, 0);
	m_callSteamLeaderboardScoreUploaded.Set(hSteamAPICall, this, &UUploadScoreToSteamLeaderboard::OnSteamLeaderboardScoreUploaded);
}
void UUploadScoreToSteamLeaderboard::OnSteamLeaderboardScoreUploaded(LeaderboardScoreUploaded_t* pCallback, bool bIOFailure)
{
	if (pCallback->m_bSuccess == 1 && !bIOFailure)
	{
		onSuccess.Broadcast(score);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IO Failure! (Failed to upload score)"));

		onFailure.Broadcast(NULL);
		RemoveFromRoot();
	}

	RemoveFromRoot();
}

