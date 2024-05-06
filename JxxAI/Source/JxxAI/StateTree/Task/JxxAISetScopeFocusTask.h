// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "JxxAISetScopeFocusTask.generated.h"




USTRUCT()
struct FJxxAISetScopeFocusTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bUseFocusActor;

	UPROPERTY(EditAnywhere, Category = Parameter,meta = (EditCondition="bUseFocusActor"))
	TObjectPtr<AActor> FocusActor;

	UPROPERTY(EditAnywhere, Category = Parameter,meta = (EditCondition="!bUseFocusActor"))
	FVector FocalPoint;

	// 0对应着Gameplay,仅在你明确知道这个意义为何的情况下才应该增加这个值.
	UPROPERTY(EditAnywhere, Category = Parameter,meta = (ClampMin = "0", UIMin = "0"))
	uint8 AIFocusPriority;

	TOptional<uint8> AppliedAIFocusPriority;
};





/**
 * 
 */
USTRUCT(meta = (DisplayName = "AI Set Scope Focus"), Category = "AI")
struct JXXAI_API FJxxAISetScopeFocusTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxAISetScopeFocusTaskInstanceData;

	FJxxAISetScopeFocusTask()
	{
		bShouldCallTick = false;

		// Do not clear the request on sustained transitions.
		// A child state (move) task can succeed on the same tick as the request is made (very likely in event based ticking).
		// That will cause transitions which would kill out request immediately.
		bShouldStateChangeOnReselect = false;
	}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	/** Handle to retrieve AIController. */
	TStateTreeExternalDataHandle<class AAIController> AIControllerHandle;
	
};
