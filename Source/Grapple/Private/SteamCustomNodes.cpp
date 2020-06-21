// Copyright Weston Mitchell 2020


#include "SteamCustomNodes.h"

// Include SteamAPI and ignore useless warnings
#if USING_CODE_ANALYSIS
MSVC_PRAGMA(warning(push))
MSVC_PRAGMA(warning(disable : ALL_CODE_ANALYSIS_WARNINGS))
#endif // USING_CODE_ANALYSIS
#pragma warning(push)
#pragma warning(disable:4996)
#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT
#include "ThirdParty/Steamworks/Steamv146/sdk/public/steam/steam_api.h"
#pragma pop_macro("ARRAY_COUNT")
#pragma warning(pop)

// Takes AppID of DLC and checks if the user owns the DLC & if the DLC is installed
bool USteamCustomNodes::BIsDlcInstalled(int32 AppID)
{
	if (SteamUser() != nullptr)
	{
		bool Out = SteamApps()->BIsDlcInstalled(AppID);
		return Out;
	}
	return false;
}

// Returns true if that app is installed (not necessarily owned)
bool USteamCustomNodes::BIsAppInstalled(int32 AppID)
{
	if (SteamUser() != nullptr)
	{
		bool Out = SteamApps()->BIsAppInstalled(AppID);
		return Out;
	}
	return false;
}

void USteamCustomNodes::OpenStore(int32 DLC_AppID)
{
	if (SteamUser() != nullptr)
	{
		// SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/");
		SteamFriends()->ActivateGameOverlayToStore(DLC_AppID, k_EOverlayToStoreFlag_None);
	}
}