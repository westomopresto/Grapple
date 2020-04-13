// Copyright Weston Mitchell, LLC 2020


#include "GetPort.h"


const INT UGetPort::GetPort(UObject* WorldContextObject) {
	
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			return World->URL.Port;
		}
	}
	return 0;
}