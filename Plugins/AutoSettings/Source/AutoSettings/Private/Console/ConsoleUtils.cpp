// Copyright Sam Bonifacio. All Rights Reserved.

#include "Console/ConsoleUtils.h"
#include "Logging/MessageLog.h"
#include "Misc/AutoSettingsError.h"

IConsoleVariable* UConsoleUtils::GetCVar(FName Name)
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name.ToString().GetCharArray().GetData());

	if (!CVar)
	{
		FAutoSettingsError::LogError(FString::Printf(TEXT("Tried to find console variable '%s' that doesn't exist"), *Name.ToString()));
	}

	return CVar;
}

bool UConsoleUtils::IsCVarRegistered(FName Name)
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name.ToString().GetCharArray().GetData());

	return CVar != nullptr;
}

void UConsoleUtils::RegisterIntCVar(FName Name, int32 DefaultValue, const FString& Help)
{
	FString SanitizedHelp = Help.IsEmpty() ? " " : Help;
	IConsoleManager::Get().RegisterConsoleVariable(Name.ToString().GetCharArray().GetData(), DefaultValue, SanitizedHelp.GetCharArray().GetData(), 0);
}

void UConsoleUtils::RegisterBoolCVar(FName Name, bool DefaultValue, const FString& Help)
{
	FString SanitizedHelp = Help.IsEmpty() ? " " : Help;
	IConsoleManager::Get().RegisterConsoleVariable(Name.ToString().GetCharArray().GetData(), DefaultValue, SanitizedHelp.GetCharArray().GetData(), 0);
}

void UConsoleUtils::RegisterFloatCVar(FName Name, float DefaultValue, const FString& Help)
{
	FString SanitizedHelp = Help.IsEmpty() ? " " : Help;
	IConsoleManager::Get().RegisterConsoleVariable(Name.ToString().GetCharArray().GetData(), DefaultValue, SanitizedHelp.GetCharArray().GetData(), 0);
}

void UConsoleUtils::RegisterStringCVar(FName Name, const FString& DefaultValue, const FString& Help)
{
	FString SanitizedHelp = Help.IsEmpty() ? " " : Help;
	IConsoleManager::Get().RegisterConsoleVariable(Name.ToString().GetCharArray().GetData(), DefaultValue, SanitizedHelp.GetCharArray().GetData(), 0);
}

int32 UConsoleUtils::GetIntCVar(FName Name)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
		return CVar->GetInt();
	return 0;
}

bool UConsoleUtils::GetBoolCVar(FName Name)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
		return CVar->GetBool();
	return false;
}

float UConsoleUtils::GetFloatCVar(FName Name)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
		return CVar->GetFloat();
	return 0.0;
}

FString UConsoleUtils::GetStringCVar(FName Name)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
		return CVar->GetString();
	return FString();
}

void UConsoleUtils::SetIntCVar(FName Name, int32 Value)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
	{
		CVar->Set(Value, PreserveFlags(CVar));
		IConsoleManager::Get().CallAllConsoleVariableSinks();
	}
}

void UConsoleUtils::SetBoolCVar(FName Name, bool Value)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
	{
		CVar->Set(Value, PreserveFlags(CVar));
		IConsoleManager::Get().CallAllConsoleVariableSinks();
	}
}

void UConsoleUtils::SetFloatCVar(FName Name, float Value)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
	{
		CVar->Set(Value, PreserveFlags(CVar));
		IConsoleManager::Get().CallAllConsoleVariableSinks();
	}
}

void UConsoleUtils::SetStringCVar(FName Name, FString Value)
{
	IConsoleVariable* CVar = GetCVar(Name);
	if (CVar)
	{
		CVar->Set(Value.GetCharArray().GetData(), PreserveFlags(CVar));
		IConsoleManager::Get().CallAllConsoleVariableSinks();
	}
}

EConsoleVariableFlags UConsoleUtils::PreserveFlags(IConsoleVariable* CVar)
{
	return (EConsoleVariableFlags)(CVar->GetFlags() & ECVF_SetByMask);
}
