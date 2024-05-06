// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/NoExportTypes.h"
#include "JxxTimeLimitTask.generated.h"



USTRUCT()
struct FJxxTimeLimitTaskInstanceData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = Parameter, meta = (ClampMin = "0.0"))
	float Duration = 1.f;

	/** Internal countdown in seconds. */
	float RemainingTime = 0.f;
};



USTRUCT(meta = (DisplayName = "Time Limit"), Category = "Flow")
struct JXXAI_API FJxxTimeLimitTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxTimeLimitTaskInstanceData;

	FJxxTimeLimitTask()
	{
		bTimeOutAsSuccess = false;
	}

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bTimeOutAsSuccess;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	//virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
};
