// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxClaimSmartObjectTask.h"

#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

FJxxClaimSmartObjectTask::FJxxClaimSmartObjectTask()
{
	bShouldStateChangeOnReselect = false;
}

EStateTreeRunStatus FJxxClaimSmartObjectTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);

	FSmartObjectClaimHandle ClaimedSlot = SmartObjectSubsystem.MarkSlotAsClaimed(InstanceData.SlotHandle, InstanceData.ClaimPriority);
	if (ClaimedSlot.IsValid())
	{
		InstanceData.ClaimedHandle = ClaimedSlot;
	}

	return InstanceData.ClaimedHandle.IsValid() ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FJxxClaimSmartObjectTask::Tick(FStateTreeExecutionContext& Context,const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);

	if (InstanceData.ClaimedHandle.IsValid())
	{
		if (!SmartObjectSubsystem.IsClaimedSmartObjectValid(InstanceData.ClaimedHandle))
		{
			InstanceData.ClaimedHandle.Invalidate();
		}
	}

	return InstanceData.ClaimedHandle.IsValid() ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

bool FJxxClaimSmartObjectTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(SmartObjectSubsystemHandle);
	return true;
}

void FJxxClaimSmartObjectTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);

	SmartObjectSubsystem.Release(InstanceData.ClaimedHandle);
}
