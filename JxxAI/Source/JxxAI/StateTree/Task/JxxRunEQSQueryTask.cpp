// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxRunEQSQueryTask.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Components/StateTreeComponent.h"


UJxxRunEQSQueryTaskInstanceData::UJxxRunEQSQueryTaskInstanceData()
{
	RequestID = INDEX_NONE;

	OutQueryResult = EEnvQueryStatus::Failed;
	OutEnvQueryStatus = EEnvQueryStatus::Failed;

	bOutQuerySuccess=false;
}

void UJxxRunEQSQueryTaskInstanceData::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	ensure(RequestID != INDEX_NONE);

	OutQueryResult = *Result;
	OutEnvQueryStatus = OutQueryResult.GetRawStatus();

	RequestID = INDEX_NONE;

	bOutQuerySuccess = OutEnvQueryStatus == EEnvQueryStatus::Success;
}

#if WITH_EDITOR
void UJxxRunEQSQueryTaskInstanceData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty &&
		PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UJxxRunEQSQueryTaskInstanceData, EQSRequest))
	{
		EQSRequest.PostEditChangeProperty(*this, PropertyChangedEvent);
	}
}
#endif

EStateTreeRunStatus FJxxRunEQSQueryTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	if (InstanceData->Querier)
	{
		FQueryFinishedSignature QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(InstanceData, &UJxxRunEQSQueryTaskInstanceData::OnQueryFinished);

		InstanceData->RequestID=	InstanceData->EQSRequest.Execute(*InstanceData->Querier, QueryFinishedDelegate);
		const bool bIsValid = (InstanceData->RequestID >= 0);
		if (bIsValid)
		{
			ResultStatus = EStateTreeRunStatus::Running;
			InstanceData->OutQueryResult = EEnvQueryStatus::Processing;  //EQS必定会延迟到下一个Tick才会执行查询,所以在这里不用担心值被污染.
		}
	}
	return ResultStatus;
}

EStateTreeRunStatus FJxxRunEQSQueryTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	if (InstanceData->OutEnvQueryStatus == EEnvQueryStatus::Processing)
	{
		return EStateTreeRunStatus::Running;
	}
	else if (InstanceData->OutEnvQueryStatus == EEnvQueryStatus::Success)
	{
		return EStateTreeRunStatus::Running; //需要作为持续提供数据的节点使用.
	}
	else
	{
		return EStateTreeRunStatus::Failed; //任何其他情况都认定为失败.
	}
}

void FJxxRunEQSQueryTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	const bool bIsValid = (InstanceData->RequestID >= 0);
	if (bIsValid)
	{
		UStateTreeComponent* StateTreeComp = Context.GetExternalDataPtr(StateTreeCompHandle);
		UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(StateTreeComp);
		if (QueryManager)
		{
			QueryManager->AbortQuery(InstanceData->RequestID);
		}
	}
	InstanceData->RequestID = INDEX_NONE;
}

bool FJxxRunEQSQueryTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(StateTreeCompHandle);
	return true;
}
