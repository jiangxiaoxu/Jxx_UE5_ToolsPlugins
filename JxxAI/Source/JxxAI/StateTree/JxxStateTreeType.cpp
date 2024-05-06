// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxStateTreeType.h"

FJxxSTLocationEntry::FJxxSTLocationEntry()
{
	LocationType = EJxxStateTreeLocationEntryType::UnSet;
}

FJxxSTLocationEntry::FJxxSTLocationEntry(AActor* InActor)
{
	LocationType = EJxxStateTreeLocationEntryType::ActorTransform;
	Actor = InActor;
}

FJxxSTLocationEntry::FJxxSTLocationEntry(const FTransform& InTransform)
{
	LocationType = EJxxStateTreeLocationEntryType::LiteralTransform;
	LiteralTransform = InTransform;
}

FJxxSTLocationEntry::FJxxSTLocationEntry(const FVector& InLocation)
{
	LocationType = EJxxStateTreeLocationEntryType::LiteralTransform;
	LiteralTransform.SetLocation(InLocation);
}

FJxxSTLocationEntry::FJxxSTLocationEntry(USceneComponent* InComp,FName InSocketName)
{
	LocationType = EJxxStateTreeLocationEntryType::SocketTransform;
	Component = InComp;
	SocketName = InSocketName;
}

bool FJxxSTLocationEntry::IsValid() const
{
	return GetTransform().IsSet();
}

TOptional<FTransform> FJxxSTLocationEntry::GetTransform() const
{
	TOptional<FTransform> Result;

	if (LocationType==EJxxStateTreeLocationEntryType::ActorTransform)
	{
		if (Actor)
		{
			Result = Actor->GetTransform();
		}
	}
	else if(LocationType == EJxxStateTreeLocationEntryType::SocketTransform)
	{
		if (Component)
		{
			Result = Component->GetSocketTransform(SocketName);
		}
	}
	else if(LocationType == EJxxStateTreeLocationEntryType::LiteralTransform)
	{
		Result = LiteralTransform;
	}

	return Result;
}

