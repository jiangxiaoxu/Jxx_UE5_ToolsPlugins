// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "JxxWaitEQSQueryResultTask.generated.h"




USTRUCT()
struct FJxxWaitEQSQueryResultTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FEnvQueryResult WaittingQueryResult;
};










USTRUCT(meta = (DisplayName = "Wait EQS Query Result"), Category = "EQS")
struct JXXAI_API FJxxWaitEQSQueryResultTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxWaitEQSQueryResultTaskInstanceData;

	FJxxWaitEQSQueryResultTask()
	{
	}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

};
