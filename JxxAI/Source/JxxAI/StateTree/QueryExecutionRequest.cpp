// Fill out your copyright notice in the Description page of Project Settings.


#include "QueryExecutionRequest.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"


void FJxxStateTreeDynamicParam::GenerateConfigurableParamsFromNamedValues(UObject& QueryOwner,TArray<FJxxStateTreeDynamicParam>& OutQueryConfig, TArray<FEnvNamedValue>& InQueryParams)
{
	for (const auto&[ParamName,ParamType,Value] : InQueryParams)
	{
		FJxxStateTreeDynamicParam& NewParam = OutQueryConfig[OutQueryConfig.AddDefaulted()];
		NewParam.ParamName = ParamName;
		NewParam.ParamType = ParamType;
		NewParam.Value = Value;
	}
}

FJxxStateTreeDynamicParam::operator FAIDynamicParam() const
{
	FAIDynamicParam Result;
	Result.ParamName = ParamName;
	Result.ParamType = ParamType;
	Result.Value = Value;

	return Result;
}

FJxxStateTreeEQSExecutionRequest::FJxxStateTreeEQSExecutionRequest()
{
	QueryTemplate = nullptr;
	RunMode = EEnvQueryRunMode::SingleResult;
}

int32 FJxxStateTreeEQSExecutionRequest::Execute(UObject& QueryOwner,
                                                FQueryFinishedSignature& QueryFinishedDelegate)
{
	if (QueryTemplate != nullptr)
	{
		FEnvQueryRequest QueryRequest(QueryTemplate, &QueryOwner);
		if (QueryConfig.Num() > 0)
		{
			// resolve 
			for (FJxxStateTreeDynamicParam& RuntimeParam : QueryConfig)
			{
				QueryRequest.SetDynamicParam(RuntimeParam);
			}
		}

		return QueryRequest.Execute(RunMode, QueryFinishedDelegate);
	}

	return INDEX_NONE;
}

#if WITH_EDITOR
void FJxxStateTreeEQSExecutionRequest::PostEditChangeProperty(UObject& Owner,FPropertyChangedEvent& PropertyChangedEvent)
{
	check(PropertyChangedEvent.MemberProperty);
	check(PropertyChangedEvent.Property);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(FJxxStateTreeEQSExecutionRequest, QueryTemplate))
	{
		if (QueryTemplate)
		{
			TArray<FAIDynamicParam> TempParams = ConverToAnotherTypeArray<FJxxStateTreeDynamicParam, FAIDynamicParam>(QueryConfig);
			QueryTemplate->CollectQueryParams(Owner, TempParams);
			QueryConfig = ConverToAnotherTypeArray<FAIDynamicParam,FJxxStateTreeDynamicParam>(TempParams);
		}
		else
		{
			QueryConfig.Reset();
		}
	}
}
#endif // WITH_EDITOR
