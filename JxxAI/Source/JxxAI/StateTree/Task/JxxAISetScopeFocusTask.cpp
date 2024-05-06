// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxAISetScopeFocusTask.h"

#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

#include "AIController.h"

EStateTreeRunStatus FJxxAISetScopeFocusTask::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	AAIController* MyController = Context.GetExternalDataPtr(AIControllerHandle);

	const uint8 ValidAIFocusPriority = EAIFocusPriority::Gameplay + FMath::Max<uint8>(InstanceData.AIFocusPriority, 0);

	if (MyController)
	{
		if (InstanceData.bUseFocusActor)
		{
			if (InstanceData.FocusActor)
			{
				MyController->SetFocus(InstanceData.FocusActor,ValidAIFocusPriority);
				InstanceData.AppliedAIFocusPriority = ValidAIFocusPriority;
				ResultStatus = EStateTreeRunStatus::Running;
			}
		}
		else
		{
			MyController->SetFocalPoint(InstanceData.FocalPoint,ValidAIFocusPriority);
			InstanceData.AppliedAIFocusPriority = ValidAIFocusPriority;
			ResultStatus = EStateTreeRunStatus::Running;
		}
	}

	return ResultStatus;
}

bool FJxxAISetScopeFocusTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(AIControllerHandle);
	return true;
}

void FJxxAISetScopeFocusTask::ExitState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	AAIController* MyController = Context.GetExternalDataPtr(AIControllerHandle);
	if (MyController)
	{
		if (InstanceData.AppliedAIFocusPriority.IsSet())
		{
			MyController->ClearFocus(InstanceData.AppliedAIFocusPriority.GetValue());
		}
	}
}
