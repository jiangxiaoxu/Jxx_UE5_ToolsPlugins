// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxTimeLimitTask.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FJxxTimeLimitTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.RemainingTime = FMath::Max(0, InstanceData.Duration);


	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FJxxTimeLimitTask::Tick(FStateTreeExecutionContext& Context,const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.RemainingTime -= DeltaTime;
	if (InstanceData.RemainingTime <= 0.f)
	{
		return bTimeOutAsSuccess ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
	}
	return EStateTreeRunStatus::Running;
}

