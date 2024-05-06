// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "JxxStateTreeSomethingBlueprintBase.generated.h"

/**
 * 
 */
UCLASS()
class JXXAI_API UJxxStateTreeTaskBlueprintBase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()


protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> ContextActor;

	UPROPERTY(BlueprintReadOnly)
	TArray<FStateTreeEvent> EventToProcess;


	
};


UCLASS()
class JXXAI_API UJxxStateTreeEvaluatorBlueprintBase : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()


protected:
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> ContextActor;

};


UCLASS()
class JXXAI_API UJxxStateTreeConditionBlueprintBase : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()


protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const;

	UPROPERTY(BlueprintReadOnly)
	mutable TWeakObjectPtr<AActor> ContextActor;

};
