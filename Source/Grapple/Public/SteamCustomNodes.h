// Copyright Weston Mitchell 2020

#pragma once

// Include SteamAPI and ignore useless warnings
/*#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:4828)
#pragma warning(disable:4265)
#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT
#pragma pop_macro("ARRAY_COUNT")
#pragma warning(pop)
*/
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SteamCustomNodes.generated.h"


UCLASS()
class GRAPPLE_API USteamCustomNodes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, META = (DisplayName = "Has DLC Installed", CompactNodeTitle = "Has DLC Installed", Category = "Steam Custom Nodes"))
		static bool BIsDlcInstalled(int32 AppID);

	UFUNCTION(BlueprintCallable, META = (DisplayName = "Game Installed", CompactNodeTitle = "Game Installed", Category = "Steam Custom Nodes"))
		static bool BIsAppInstalled(int32 AppID);

	UFUNCTION(BlueprintCallable, META = (DisplayName = "Open Store", CompactNodeTitle = "Open Store", Category = "Steam Custom Nodes"))
		static void OpenStore(int32 DLC_AppID);
};

