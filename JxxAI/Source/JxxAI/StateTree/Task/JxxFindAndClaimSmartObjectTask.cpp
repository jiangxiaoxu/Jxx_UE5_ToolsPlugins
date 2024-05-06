// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxFindAndClaimSmartObjectTask.h"

#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

FJxxFindAndClaimSmartObjectTask::FJxxFindAndClaimSmartObjectTask()
{
	bShouldStateChangeOnReselect = false;
}

EStateTreeRunStatus FJxxFindAndClaimSmartObjectTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);

	FSmartObjectRequestResult FindResult = SmartObjectSubsystem.FindSmartObject(InstanceData.Request);
	if (FindResult.IsValid())
	{
		FSmartObjectClaimHandle ClaimedSlot = SmartObjectSubsystem.MarkSlotAsClaimed(FindResult.SlotHandle);
		if (ClaimedSlot.IsValid())
		{
			InstanceData.ClaimedHandle = ClaimedSlot;
		}
	}

	return InstanceData.ClaimedHandle.IsValid() ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FJxxFindAndClaimSmartObjectTask::Tick(FStateTreeExecutionContext& Context,const float DeltaTime) const
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

bool FJxxFindAndClaimSmartObjectTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(SmartObjectSubsystemHandle);
	return true;
}

void FJxxFindAndClaimSmartObjectTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);

	SmartObjectSubsystem.Release(InstanceData.ClaimedHandle);
}
