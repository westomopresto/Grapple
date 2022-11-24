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
#include "FOCleanupLibrary.generated.h"

UCLASS()
class GRAPPLE_API UFOCleanupLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Optimization", meta = (DefaultToSelf = Object))
		static void DestroyObject(UObject* Object);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAppVersion"), Category = "Game Config")
		static FString GetAppVersion();
};

