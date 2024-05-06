// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxWaitEQSQueryResultTask.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FJxxWaitEQSQueryResultTask::EnterState(FStateTreeExecutionContext& Context,
                                                           const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	if (!InstanceData.WaittingQueryResult.IsFinished())
	{
		ResultStatus = EStateTreeRunStatus::Running;
	}
	else if (InstanceData.WaittingQueryResult.IsSuccessful())
	{
		ResultStatus = EStateTreeRunStatus::Succeeded;
	}

	return ResultStatus;
}

EStateTreeRunStatus FJxxWaitEQSQueryResultTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	if (!InstanceData.WaittingQueryResult.IsFinished())
	{
		ResultStatus = EStateTreeRunStatus::Running;
	}
	else if (InstanceData.WaittingQueryResult.IsSuccessful())
	{
		ResultStatus = EStateTreeRunStatus::Succeeded;
	}

	return ResultStatus;
}
