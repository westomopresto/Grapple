// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#include "ConfigFile.h"
#include "ConfigBPPlugin.h"

void UConfigFile::InitializeObject(FString path, EFileOpenState access)
{
	filePath = path;
	accessLevel = access;
}

FString UConfigFile::GetFilePath()
{
	return filePath;
}
