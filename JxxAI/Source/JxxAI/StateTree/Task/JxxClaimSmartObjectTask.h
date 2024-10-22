﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "SmartObjectSubsystem.h"
#include "JxxClaimSmartObjectTask.generated.h"

/**
 * 
 */
USTRUCT()
struct JXXAI_API FJxxClaimSmartObjectTaskInstanceData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Input)
	FSmartObjectHandle SmartObjectHandle;

	//SlotHandle Handle to a smart object slot.
	UPROPERTY(EditAnywhere, Category = Input)
	FSmartObjectSlotHandle SlotHandle;

	//ClaimPriority Claim priority, a slot claimed at lower priority can be claimed by higher priority (unless already in use).
	UPROPERTY(EditAnywhere, Category = Parameter)
	ESmartObjectClaimPriority ClaimPriority = ESmartObjectClaimPriority::Normal;

	//return A handle binding the claimed smart object, its slot and a user id.
	UPROPERTY(EditAnywhere, Category = Output)
	FSmartObjectClaimHandle ClaimedHandle;
};

USTRUCT(meta = (DisplayName = "Claim Smart Object"), Category = "Smart Object")
struct JXXAI_API FJxxClaimSmartObjectTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxClaimSmartObjectTaskInstanceData;

	FJxxClaimSmartObjectTask();

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	//virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus,const FStateTreeActiveStates& CompletedActiveStates) const override;

	TStateTreeExternalDataHandle<USmartObjectSubsystem> SmartObjectSubsystemHandle;

};