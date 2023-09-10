// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#pragma once

#include "Engine/UserDefinedEnum.h"
#include "FileOpenState.generated.h"

/**
 * Enum for storing file permission states
 */
UENUM(BlueprintType)
enum class EFileOpenState : uint8
{
	FOS_ReadWrite	UMETA(DisplayName = "Read & Write"),
	FOS_ReadOnly	UMETA(DisplayName = "Read Only"),
	FOS_Error		UMETA(DisplayName = "No Access (Error)"),
};