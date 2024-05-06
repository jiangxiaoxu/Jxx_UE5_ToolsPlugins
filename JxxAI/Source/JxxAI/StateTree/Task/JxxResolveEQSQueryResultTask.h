// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObjectTypes.h"
#include "StateTreeTaskBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "JxxAI/StateTree/JxxStateTreeType.h"
#include "UObject/NoExportTypes.h"
#include "JxxResolveEQSQueryResultTask.generated.h"



USTRUCT()
struct  FJxxResolveEQSQueryResultAsActorTaskInstanceData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Input)
	FEnvQueryResult QueryResult;

	UPROPERTY(VisibleAnywhere, Category = Output)
	TObjectPtr<AActor> OutActor;
};




USTRUCT(meta = (DisplayName = "EQS Result As Actor"), Category = "EQS")
struct JXXAI_API FJxxResolveEQSQueryResultAsActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxResolveEQSQueryResultAsActorTaskInstanceData;

	FJxxResolveEQSQueryResultAsActorTask()
	{
		bShouldCallTick = false;
	}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};

/**********************************************************************************************************************************************/
USTRUCT()
struct  FJxxResolveEQSQueryResultAsVectorTaskInstanceData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Input)
	FEnvQueryResult QueryResult;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FVector OutPoint;
};


USTRUCT(meta = (DisplayName = "EQS Result As Vector"), Category = "EQS")
struct JXXAI_API FJxxResolveEQSQueryResultAsVectorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxResolveEQSQueryResultAsVectorTaskInstanceData;

	FJxxResolveEQSQueryResultAsVectorTask()
	{
		bShouldCallTick = false;
	}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};



USTRUCT()
struct  FJxxResolveEQSQueryResultAsSmartObjectTaskInstanceData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Input)
	FEnvQueryResult QueryResult;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FJxxSTLocationEntry LocationEntry;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FSmartObjectHandle SmartObjectHandle;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FSmartObjectSlotHandle SlotHandle;
};


USTRUCT(meta = (DisplayName = "EQS Result As SmartObject"), Category = "EQS")
struct JXXAI_API FJxxResolveEQSQueryResultAsSmartObjectTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxResolveEQSQueryResultAsSmartObjectTaskInstanceData;

	FJxxResolveEQSQueryResultAsSmartObjectTask()
	{
		bShouldCallTick = false;
	}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};


//******************************************************



USTRUCT()
struct  FJxxResolveEQSQueryResultAsLocationEntryTaskInstanceData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Input)
	FEnvQueryResult QueryResult;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FJxxSTLocationEntry LocationEntry;
};




USTRUCT(meta = (DisplayName = "EQS Result As LocationEntry"), Category = "EQS")
struct JXXAI_API FJxxResolveEQSQueryResultAsLocationEntryTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxResolveEQSQueryResultAsLocationEntryTaskInstanceData;

	FJxxResolveEQSQueryResultAsLocationEntryTask()
	{
		bShouldCallTick = false;
	}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};