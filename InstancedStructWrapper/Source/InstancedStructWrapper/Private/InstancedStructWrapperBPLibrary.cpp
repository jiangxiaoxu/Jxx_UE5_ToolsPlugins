// Copyright Epic Games, Inc. All Rights Reserved.

#include "InstancedStructWrapperBPLibrary.h"
#include "InstancedStructWrapper.h"


FInstancedStruct UInstancedStructWrapperBPLibrary::GetInstancedStructFromGameplayEventData(const FGameplayEventData& EventData)
{
	FInstancedStruct Result;
	if (auto Obj=Cast<UInstancedStructWrapperObject>(EventData.OptionalObject2))
	{
		Result = Obj->InstancedStruct;
	}
	return Result;
}
