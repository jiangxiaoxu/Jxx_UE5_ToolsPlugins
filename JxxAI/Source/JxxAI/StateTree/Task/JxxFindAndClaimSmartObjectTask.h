// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "SmartObjectSubsystem.h"
#include "JxxFindAndClaimSmartObjectTask.generated.h"

/**
 * 
 */
USTRUCT()
struct JXXAI_API FJxxFindAndClaimSmartObjectTaskInstanceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Parameter)
	FSmartObjectRequest Request;

	//ClaimPriority Claim priority, a slot claimed at lower priority can be claimed by higher priority (unless already in use).
	UPROPERTY(EditAnywhere, Category = Parameter)
	ESmartObjectClaimPriority ClaimPriority = ESmartObjectClaimPriority::Normal;

	UPROPERTY(EditAnywhere, Category = Output)
	FSmartObjectClaimHandle ClaimedHandle;
};

USTRUCT(meta = (DisplayName = "Find And Claim Smart Object"), Category = "Smart Object")
struct JXXAI_API FJxxFindAndClaimSmartObjectTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxFindAndClaimSmartObjectTaskInstanceData;

	FJxxFindAndClaimSmartObjectTask();

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	//virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus,const FStateTreeActiveStates& CompletedActiveStates) const override;

	TStateTreeExternalDataHandle<USmartObjectSubsystem> SmartObjectSubsystemHandle;

};