// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeConditionBase.h"
#include "AITypes.h"
#include "JxxAI/StateTree/JxxStateTreeType.h"
#include "JxxDistanceCondition.generated.h"


USTRUCT()
struct JXXAI_API FJxxDistanceConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bSourceUseActor;

	UPROPERTY(EditAnywhere, Category = "Context", meta = (EditConditionHides, EditCondition="bSourceUseActor"))
	AActor* SourceActor;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditConditionHides, EditCondition = "!bSourceUseActor"))
	FJxxSTLocationEntry SourceLocationEntry;



	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bTargetUseActor;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditConditionHides, EditCondition="bTargetUseActor"))
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (EditConditionHides, EditCondition="!bTargetUseActor"))
	FJxxSTLocationEntry TargetLocationEntry;

	UPROPERTY(EditAnywhere, Category = "Parameter",meta=(ClampMin = "0.0", UIMin="0.0", EditConditionHides, EditCondition = "Operator!=EJxxDistanceCheck::InRange"))
	double Distance = 600;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0", EditConditionHides, EditCondition = "Operator==EJxxDistanceCheck::NearlyEqual"))
	double Tolerance = 10.0;

	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0", EditConditionHides, EditCondition = "Operator==EJxxDistanceCheck::InRange"))
	FVector2D DistanceRange;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bIgnoreZ = true;

	UPROPERTY(EditAnywhere, Category = "Condition")
	EJxxDistanceCheck Operator = EJxxDistanceCheck::Less;
};


USTRUCT(DisplayName="Distance Condition Ex")
struct JXXAI_API FJxxDistanceCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

public:
	using FInstanceDataType = FJxxDistanceConditionInstanceData;

	FJxxDistanceCondition()= default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

};
