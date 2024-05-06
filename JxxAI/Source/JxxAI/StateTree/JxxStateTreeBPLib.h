// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JxxStateTreeType.h"
#include "StateTreeEvents.h"
#include "JxxStateTreeBPLib.generated.h"






UCLASS()
class JXXAI_API UJxxStateTreeBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FJxxSTLocationEntry MakeLocationEntryByActor(AActor* Actor);

	UFUNCTION(BlueprintPure)
	static FJxxSTLocationEntry MakeLocationEntryByTransform(FTransform Transform);

	UFUNCTION(BlueprintPure)
	static FJxxSTLocationEntry MakeLocationEntryByComponent(USceneComponent* InComponent, FName SocketName);

	UFUNCTION(BlueprintPure)
	static bool IsValidLocationEntry(const FJxxSTLocationEntry& LocationEntry);

	UFUNCTION(BlueprintCallable,meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool GetTransform_Exec(const FJxxSTLocationEntry& LocationEntry,FTransform& OutTransform);

	UFUNCTION(BlueprintPure)
	static bool GetTransform(const FJxxSTLocationEntry& LocationEntry, FTransform& OutTransform);

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool GetStateTreeEventByTag(const TArray<FStateTreeEvent>& Events, UPARAM(meta = (Categories = "StateTreeEvent"))  FGameplayTag EventTag,bool bMatchesTagExact,FStateTreeEvent& OutEvent);

	UFUNCTION(BlueprintCallable)
	static void BreakStateTreeEvent(const FStateTreeEvent& Event, FGameplayTag& EventTag, FInstancedStruct& Payload, FName& Origin);

	UFUNCTION(BlueprintCallable)
	static FStateTreeEvent MakeStateTreeEvent(FGameplayTag EventTag, FInstancedStruct Payload, FName Origin);

};
