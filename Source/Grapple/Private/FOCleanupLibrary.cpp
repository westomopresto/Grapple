// Copyright Weston Mitchell 2020


#include "FOCleanupLibrary.h"
#include <ctime>

// Include SteamAPI and ignore useless warnings
#if USING_CODE_ANALYSIS
MSVC_PRAGMA(warning(push))
MSVC_PRAGMA(warning(disable : ALL_CODE_ANALYSIS_WARNINGS))
#endif // USING_CODE_ANALYSIS
#pragma warning(push)
#pragma warning(disable:4996)
#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT
#pragma pop_macro("ARRAY_COUNT")
#pragma warning(pop)


void UFOCleanupLibrary::DestroyObject(UObject* Object) {
	if (Object)
	{
		Object->ConditionalBeginDestroy();
	}
}

FString UFOCleanupLibrary::GetAppVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}

int32 UFOCleanupLibrary::GetDayOfWeek()
{
	time_t now = time(0);
	tm* localTime = localtime(&now);
	return localTime->tm_wday;
}