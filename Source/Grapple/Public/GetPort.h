// Copyright Weston Mitchell, LLC 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GetPort.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLE_API UGetPort : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject", Friendly = "Get Server Port", CompactNodeTitle = "GetPort"), Category = "Server Info")
		static const int GetPort(UObject* WorldContextObject);
};