﻿// Copyright Sam Bonifacio. All Rights Reserved.

#include "Misc/AutoSettingsError.h"
#include "Logging/MessageLog.h"
#include "AutoSettings.h"
#include "UObject/Class.h"

void FAutoSettingsError::LogError(const FString& Message)
{
	// PIE message log
	FMessageLog("PIE").Error(FText::FromString("Auto Settings: " + Message));
	// Output log
	UE_LOG(LogAutoSettings, Error, TEXT("%s"), *Message);
}

void FAutoSettingsError::LogInvalidClass(const FString& SourceObjectName, const FString& ClassPropertyName)
{
	LogError(FString::Printf(TEXT("%s has an invalid %s. Please update the property to a valid class."), *SourceObjectName, *ClassPropertyName));
}

void FAutoSettingsError::LogInvalidClass(UClass* SourceClass, const FString& ClassPropertyName)
{
	LogInvalidClass(GetSafeClassName(SourceClass), ClassPropertyName);
}

void FAutoSettingsError::LogWidgetCreationFailed(UClass* SourceClass, const FString& WidgetName)
{
	LogError(FString::Printf(TEXT("%s: Failed to create %s widget"), *GetSafeClassName(SourceClass), *WidgetName));
}

FString FAutoSettingsError::GetSafeClassName(UClass* Class)
{
	return Class ? Class->GetName() : "None";
}
