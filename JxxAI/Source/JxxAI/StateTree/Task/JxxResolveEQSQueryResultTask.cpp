// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxResolveEQSQueryResultTask.h"

#include "EnvQueryItemType_SmartObject.h"
#include "StateTreeExecutionContext.h"


EStateTreeRunStatus FJxxResolveEQSQueryResultAsActorTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	const FEnvQueryResult& QueryResult = InstanceData.QueryResult;

	if (QueryResult.IsSuccessful() && QueryResult.Items.Num() > 0)
	{
		InstanceData.OutActor = QueryResult.GetItemAsActor(0);
		ResultStatus = EStateTreeRunStatus::Running;
	}

	return ResultStatus;
}

EStateTreeRunStatus FJxxResolveEQSQueryResultAsVectorTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	const FEnvQueryResult& QueryResult = InstanceData.QueryResult;

	if (QueryResult.IsSuccessful() && QueryResult.Items.Num() > 0)
	{
		InstanceData.OutPoint = QueryResult.GetItemAsLocation(0);
		ResultStatus = EStateTreeRunStatus::Running;
	}

	return ResultStatus;
}

EStateTreeRunStatus FJxxResolveEQSQueryResultAsSmartObjectTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	const FEnvQueryResult& QueryResult = InstanceData.QueryResult;

	if (QueryResult.IsSuccessful() && QueryResult.Items.Num() > 0)
	{
		if (QueryResult.ItemType && QueryResult.ItemType->IsChildOf(UEnvQueryItemType_SmartObject::StaticClass()))
		{
			auto Result = QueryResult.GetItemAsTypeChecked<UEnvQueryItemType_SmartObject>(0);

			InstanceData.Location = Result.Location;
			InstanceData.LocationEntry = Result.Location;
			InstanceData.SmartObjectHandle = Result.SmartObjectHandle;
			InstanceData.SlotHandle = Result.SlotHandle;
			ResultStatus = EStateTreeRunStatus::Running;
		}
	}

	return ResultStatus;
}

EStateTreeRunStatus FJxxResolveEQSQueryResultAsLocationEntryTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	const FEnvQueryResult& QueryResult = InstanceData.QueryResult;

	if (QueryResult.IsSuccessful() && QueryResult.Items.Num() > 0)
	{
		if (AActor* Actor = QueryResult.GetItemAsActor(0))
		{
			InstanceData.LocationEntry = Actor;
			ResultStatus = EStateTreeRunStatus::Running;
		}
		else
		{
			FVector Loc = QueryResult.GetItemAsLocation(0);
			InstanceData.LocationEntry = FTransform(Loc);
			ResultStatus = EStateTreeRunStatus::Running;
		}
	}

	return ResultStatus;
}