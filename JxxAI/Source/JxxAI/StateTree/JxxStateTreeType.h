// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JxxStateTreeType.generated.h"

UENUM(BlueprintType)
enum class EJxxStateTreeLocationEntryType :uint8
{
	UnSet /*UMETA(Hidden)*/,
	LiteralTransform,
	ActorTransform,
	SocketTransform
};


UENUM(meta=(DisplayName="DistanceCheck"))
enum class EJxxDistanceCheck : uint8
{
	Less,
	NearlyEqual,
	Greater,
	InRange,

	MAX UMETA(Hidden)
};






USTRUCT(BlueprintType, meta = (DisplayName = "ST Location Entry"))
struct FJxxSTLocationEntry
{
	GENERATED_BODY()

public:

	FJxxSTLocationEntry();

	FJxxSTLocationEntry(AActor* InActor);

	FJxxSTLocationEntry(const FTransform& InTransform);

	FJxxSTLocationEntry(const FVector& InLocation);

	FJxxSTLocationEntry(USceneComponent* InComp, FName SocketName);




	UPROPERTY(BlueprintReadWrite)
	EJxxStateTreeLocationEntryType LocationType;

	UPROPERTY(BlueprintReadWrite)
	FTransform LiteralTransform;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Actor;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USceneComponent> Component;

	UPROPERTY(BlueprintReadWrite)
	FName SocketName;

	bool IsValid()const;

	TOptional<FTransform> GetTransform()const;

};


USTRUCT(BlueprintType, meta = (DisplayName = "ST Location Entry Warpper"))
struct FJxxSTLocationEntryWarpper
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FJxxSTLocationEntry LocationEntry;
};
