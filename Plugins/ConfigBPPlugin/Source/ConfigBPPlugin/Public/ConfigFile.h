// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "FileOpenState.h"
#include "ConfigFile.generated.h"

/**
 * Class That Stores A Configuration File For ConfigBP Usage.
 */
UCLASS(BlueprintType)
class CONFIGBPPLUGIN_API UConfigFile : public UObject
{
	GENERATED_BODY()
	
public:
	// Sets up a configuration file object using path and file permission
	void InitializeObject(FString path, EFileOpenState access);

	// Function for accessing the private filePath variable
	FString GetFilePath();
private:
	// Local variable to store that stores the absolute path to the config file
	FString filePath;
	// Local variable for storing the permission level to the above file path
	EFileOpenState accessLevel;
	
	
};
