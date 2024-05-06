// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxAIMoveToTask.h"

#include "AIController.h"
#include "AISystem.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Tasks/AITask_MoveTo.h"
#include "VisualLogger/VisualLogger.h"
#include "Components/StateTreeComponent.h"
#include "NavFilters/NavigationQueryFilter.h"

UJxxAIMoveToTaskInstanceData::UJxxAIMoveToTaskInstanceData()
{
	AcceptableRadius = GET_AI_CONFIG_VAR(AcceptanceRadius);
	bReachTestIncludesGoalRadius = bReachTestIncludesAgentRadius = GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap);
	bAllowStrafe = GET_AI_CONFIG_VAR(bAllowStrafing);
	bAllowPartialPath = GET_AI_CONFIG_VAR(bAcceptPartialPaths);
	bTrackMovingGoal = true;
	bProjectGoalLocation = true;
}

void UJxxAIMoveToTaskInstanceData::OnGameplayTaskDeactivated(UGameplayTask& InTask)
{
	// AI move task finished
	UAITask_MoveTo* MoveToTask = CastChecked<UAITask_MoveTo>(&InTask);
	MoveResultOpt = MoveToTask->GetMoveResult();
}

AActor* UJxxAIMoveToTaskInstanceData::GetGameplayTaskOwner(const UGameplayTask* InTask) const
{
	check(CachedStateTreeComp.IsValid());
	return CachedStateTreeComp->GetGameplayTaskOwner(InTask);
}

UGameplayTasksComponent* UJxxAIMoveToTaskInstanceData::GetGameplayTasksComponent(const UGameplayTask& InTask)const
{
	check(CachedStateTreeComp.IsValid());
	return CachedStateTreeComp->GetGameplayTasksComponent(InTask);
}


EStateTreeRunStatus FJxxAIMoveToTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AAIController* MyController = Context.GetExternalDataPtr(AIControllerHandle);

	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	InstanceData->CachedStateTreeComp = Context.GetExternalDataPtr(StateTreeCompHandle);

	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Running;

	if (MyController == nullptr)
	{
	//	UE_VLOG(OwnerComp.GetOwner(), LogBehaviorTree, Error, TEXT("UBTTask_MoveTo::ExecuteTask failed since AIController is missing."));
			ResultStatus = EStateTreeRunStatus::Failed;
	}
	else
	{
		ResultStatus = PerformMoveTask(Context, Transition);
	}

	return ResultStatus;

}

EStateTreeRunStatus FJxxAIMoveToTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Running;

	 if(InstanceData->MoveResultOpt.IsSet())
	{
		ResultStatus = InstanceData->MoveResultOpt.GetValue() == EPathFollowingResult::Success ? 
			EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
	}
	return ResultStatus;
}

bool FJxxAIMoveToTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(AIControllerHandle);
	Linker.LinkExternalData(StateTreeCompHandle);
	return true;
}

void FJxxAIMoveToTask::ExitState(FStateTreeExecutionContext& Context,
                                 const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	if (InstanceData->Task.IsValid())
	{
		InstanceData->Task->ExternalCancel();
	}
}

EStateTreeRunStatus FJxxAIMoveToTask::PerformMoveTask(FStateTreeExecutionContext& Context,
                                                      const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Failed;

	AAIController* MyController = Context.GetExternalDataPtr(AIControllerHandle);

	if (MyController)
	{
		FAIMoveRequest MoveReq;
		MoveReq.SetNavigationFilter(*InstanceData->FilterClass
			                            ? InstanceData->FilterClass
			                            : MyController->GetDefaultNavigationFilterClass());
		MoveReq.SetAllowPartialPath(InstanceData->bAllowPartialPath);
		MoveReq.SetAcceptanceRadius(InstanceData->AcceptableRadius);
		MoveReq.SetCanStrafe(InstanceData->bAllowStrafe);
		MoveReq.SetReachTestIncludesAgentRadius(InstanceData->bReachTestIncludesAgentRadius);
		MoveReq.SetReachTestIncludesGoalRadius(InstanceData->bReachTestIncludesGoalRadius);
		MoveReq.SetProjectGoalLocation(InstanceData->bProjectGoalLocation);
		MoveReq.SetUsePathfinding(true);

		if (InstanceData->bUseGoalActor)
		{
			if (InstanceData->GoalActor)
			{
				if (InstanceData->bTrackMovingGoal)
				{
					MoveReq.SetGoalActor(InstanceData->GoalActor);
				}
				else
				{
					MoveReq.SetGoalLocation(InstanceData->GoalActor->GetActorLocation());
				}
			}
		}
		else
		{
			MoveReq.SetGoalLocation(InstanceData->GoalLocation);
		}


		if (MoveReq.IsValid())
		{
			UAITask_MoveTo* MoveTask = InstanceData->Task.Get();
			const bool bReuseExistingTask = (MoveTask != nullptr);

			MoveTask = PrepareMoveTask(Context, Transition, MoveTask, MoveReq);
			if (MoveTask)
			{
				//InstanceData->bObserverCanFinishTask = false;

				if (bReuseExistingTask)
				{
					if (MoveTask->IsActive())
					{
						//UE_VLOG(MyController, LogBehaviorTree, Verbose, TEXT("\'%s\' reusing AITask %s"), *GetNodeName(), *MoveTask->GetName());
						MoveTask->ConditionalPerformMove();
					}
					else
					{
						//UE_VLOG(MyController, LogBehaviorTree, Verbose, TEXT("\'%s\' reusing AITask %s, but task is not active - handing over move performing to task mechanics"), *GetNodeName(), *MoveTask->GetName());
					}
				}
				else
				{
					InstanceData->Task = MoveTask;
					//	UE_VLOG(MyController, LogBehaviorTree, Verbose, TEXT("\'%s\' task implementing move with task %s"), *GetNodeName(), *MoveTask->GetName());
					MoveTask->ReadyForActivation();
				}

				//InstanceData->bObserverCanFinishTask = true;
				ResultStatus = (MoveTask->GetState() != EGameplayTaskState::Finished)? EStateTreeRunStatus::Running:
										MoveTask->WasMoveSuccessful()? EStateTreeRunStatus::Succeeded: EStateTreeRunStatus::Failed;
			}
		}
	}

	return ResultStatus;
}

UAITask_MoveTo* FJxxAIMoveToTask::PrepareMoveTask(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest)const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	AAIController* MyController = Context.GetExternalDataPtr(AIControllerHandle);
	UAITask_MoveTo* MoveTask = ExistingTask ? ExistingTask : UAITask::NewAITask<UAITask_MoveTo>(*MyController,*InstanceData);

	if (MoveTask)
	{
		MoveTask->SetUp(MoveTask->GetAIController(), MoveRequest);
	}

	return MoveTask;
}

