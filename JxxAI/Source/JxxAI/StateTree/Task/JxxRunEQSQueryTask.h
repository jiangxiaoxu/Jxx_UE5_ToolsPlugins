// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "../QueryExecutionRequest.h"
#include "JxxRunEQSQueryTask.generated.h"


UCLASS()
class JXXAI_API UJxxRunEQSQueryTaskInstanceData:public UObject
{
	GENERATED_BODY()

public:

	UJxxRunEQSQueryTaskInstanceData();


	UPROPERTY(EditAnywhere, Category = Parameter)
	FJxxStateTreeEQSExecutionRequest EQSRequest;

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> Querier;

	UPROPERTY(VisibleAnywhere, Category = Output)
	FEnvQueryResult OutQueryResult;

	UPROPERTY(VisibleAnywhere, Category = Output)
	TEnumAsByte<EEnvQueryStatus::Type> OutEnvQueryStatus;

	UPROPERTY(VisibleAnywhere, Category = Output)
	bool bOutQuerySuccess;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	/** Query request ID */
	int32 RequestID;

	TSharedPtr<FEnvQueryResult> EQSResult;





#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

};



USTRUCT(meta = (DisplayName = "Run EQS Query (Latent)"), Category = "EQS")
struct JXXAI_API FJxxRunEQSQueryTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UJxxRunEQSQueryTaskInstanceData;

	FJxxRunEQSQueryTask()
	{
		bShouldStateChangeOnReselect = false;
	}

	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual bool Link(FStateTreeLinker& Linker) override;

	TStateTreeExternalDataHandle<class UStateTreeComponent> StateTreeCompHandle;
};