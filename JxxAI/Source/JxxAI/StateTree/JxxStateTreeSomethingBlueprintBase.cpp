// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxStateTreeSomethingBlueprintBase.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus UJxxStateTreeTaskBlueprintBase::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition)
{
	ContextActor = GetOwnerActor(Context);

	EventToProcess = Context.GetEventsToProcess();

	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus UJxxStateTreeTaskBlueprintBase::Tick(FStateTreeExecutionContext& Context,const float DeltaTime)
{
	EventToProcess = Context.GetEventsToProcess();

	return Super::Tick(Context, DeltaTime);
}

void UJxxStateTreeTaskBlueprintBase::ExitState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition)
{
	EventToProcess = Context.GetEventsToProcess();

	Super::ExitState(Context, Transition);

	ContextActor = nullptr;
	EventToProcess.Empty();
}



void UJxxStateTreeEvaluatorBlueprintBase::TreeStart(FStateTreeExecutionContext& Context)
{
	ContextActor = GetOwnerActor(Context);


	Super::TreeStart(Context);
}

void UJxxStateTreeEvaluatorBlueprintBase::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);

	ContextActor = nullptr;
}

bool UJxxStateTreeConditionBlueprintBase::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (bHasTestCondition)
	{
		// Cache the owner and event queue for the duration the condition is evaluated.
		SetCachedInstanceDataFromContext(Context);

		ContextActor = GetOwnerActor(Context);

		const bool bResult = ReceiveTestCondition();

		ContextActor = nullptr;

		ClearCachedInstanceData();

		return bResult;
	}
	return false;
}