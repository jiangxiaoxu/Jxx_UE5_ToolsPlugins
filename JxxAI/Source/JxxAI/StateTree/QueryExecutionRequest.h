// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "QueryExecutionRequest.generated.h"


USTRUCT()
struct JXXAI_API FJxxStateTreeDynamicParam
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(VisibleAnywhere, Category = EQS)
	FName ParamName;

	UPROPERTY(VisibleAnywhere, Category = EQS)
	EAIParamType ParamType;

	UPROPERTY(EditAnywhere, Category = EQS)
	float Value;


	FJxxStateTreeDynamicParam()
	{
		ParamType = EAIParamType::Float;
		Value = 0.f;
	}

	FJxxStateTreeDynamicParam(const FAIDynamicParam& InAIDynamicParam)
	{
		ParamName = InAIDynamicParam.ParamName;
		ParamType = InAIDynamicParam.ParamType;
		Value = InAIDynamicParam.Value;
	}

	static void GenerateConfigurableParamsFromNamedValues(UObject &QueryOwner, TArray<FJxxStateTreeDynamicParam>& OutQueryConfig, TArray<FEnvNamedValue>& InQueryParams);

	operator FAIDynamicParam()const;

};




USTRUCT()
struct JXXAI_API FJxxStateTreeEQSExecutionRequest
{
	GENERATED_BODY()

	FJxxStateTreeEQSExecutionRequest();

	UPROPERTY(Category = Node, EditAnywhere)
	TObjectPtr<UEnvQuery> QueryTemplate;

	UPROPERTY(Category = Node, EditAnywhere)
	TArray<FJxxStateTreeDynamicParam> QueryConfig;

	/** determines which item will be stored (All = only first matching) */
	UPROPERTY(Category = Node, EditAnywhere)
	TEnumAsByte<EEnvQueryRunMode::Type> RunMode;

	int32 Execute(UObject& QueryOwner,FQueryFinishedSignature& QueryFinishedDelegate);



	template<class FromType,class ToType>
	static TArray<ToType> ConverToAnotherTypeArray(const TArray<FromType>& FromArray )
	{
		TArray<ToType> ResultArray;
		for (const auto& FromItem:FromArray)
		{
			ToType Item;
			Item.ParamName = FromItem.ParamName;
			Item.ParamType = FromItem.ParamType;
			Item.Value = FromItem.Value;

			ResultArray.Add(Item);
		}
		return ResultArray;
	}



	#if WITH_EDITOR
	void PostEditChangeProperty(UObject& Owner, struct FPropertyChangedEvent& PropertyChangedEvent);
	#endif // WITH_EDITOR
	
};
