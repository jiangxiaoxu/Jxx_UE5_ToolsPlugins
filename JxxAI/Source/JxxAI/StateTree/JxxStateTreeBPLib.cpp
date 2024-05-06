// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxStateTreeBPLib.h"

FJxxSTLocationEntry UJxxStateTreeBPLib::MakeLocationEntryByActor(AActor* Actor)
{
	return FJxxSTLocationEntry(Actor);

}

FJxxSTLocationEntry UJxxStateTreeBPLib::MakeLocationEntryByTransform(FTransform Transform)
{
	return FJxxSTLocationEntry(Transform);
}

FJxxSTLocationEntry UJxxStateTreeBPLib::MakeLocationEntryByComponent(USceneComponent* InComponent,FName SocketName)
{
	return FJxxSTLocationEntry(InComponent, SocketName);
}


bool UJxxStateTreeBPLib::IsValidLocationEntry(const FJxxSTLocationEntry& LocationEntry)
{
	return LocationEntry.IsValid();
}

bool UJxxStateTreeBPLib::GetTransform_Exec(const FJxxSTLocationEntry& LocationEntry,FTransform& OutTransform)
{
	OutTransform = FTransform::Identity;

	auto Op_Trans = LocationEntry.GetTransform();

	if (Op_Trans.IsSet())
	{
		OutTransform = Op_Trans.GetValue();
		return true;
	}
	return false;
}

bool UJxxStateTreeBPLib::GetTransform(const FJxxSTLocationEntry& LocationEntry,FTransform& OutTransform)
{
	OutTransform = FTransform::Identity;

	auto Op_Trans = LocationEntry.GetTransform();

	if (Op_Trans.IsSet())
	{
		OutTransform = Op_Trans.GetValue();
		return true;
	}
	return false;
}

bool UJxxStateTreeBPLib::GetStateTreeEventByTag(const TArray<FStateTreeEvent>& Events, FGameplayTag EventTag, bool bMatchesTagExact, FStateTreeEvent& OutEvent)
{
	OutEvent = {};

	for(const FStateTreeEvent& Event: Events)
	{
		if (bMatchesTagExact? Event.Tag.MatchesTagExact(EventTag): Event.Tag.MatchesTag(EventTag))
		{
			OutEvent = Event;
			return true;
		}
	}

	return false;
}

void UJxxStateTreeBPLib::BreakStateTreeEvent(const FStateTreeEvent& Event,FGameplayTag& EventTag,FInstancedStruct& Payload,FName& Origin)
{
	EventTag = Event.Tag;
	Payload = Event.Payload;
	Origin = Event.Origin;
}

FStateTreeEvent UJxxStateTreeBPLib::MakeStateTreeEvent(FGameplayTag EventTag,FInstancedStruct Payload,FName Origin)
{
	return FStateTreeEvent(EventTag, Payload, Origin);
}	

