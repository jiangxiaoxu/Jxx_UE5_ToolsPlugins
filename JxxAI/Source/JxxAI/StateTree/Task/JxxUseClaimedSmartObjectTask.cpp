// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxUseClaimedSmartObjectTask.h"

#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayBehavior.h"
#include "GameplayBehaviorSubsystem.h"
#include "SmartObjectComponent.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"



void UJxxUseClaimedSmartObjectTaskInstanceData::OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior,
	AActor& Avatar, const bool bInterrupted)
{
	// make sure we handle the right pawn - we can get this notify for a different
	// Avatar if the behavior sending it out is not instanced (CDO is being used to perform actions)

	if (InteractorActor && InteractorActor == &Avatar)
	{
		Behavior.GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		bBehaviorIsInterrupted = bInterrupted;
	}
}

FJxxUseClaimedSmartObjectTask::FJxxUseClaimedSmartObjectTask()
{
	bShouldStateChangeOnReselect = false;
}

EStateTreeRunStatus FJxxUseClaimedSmartObjectTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);

	bool bBehaviorActive = false;

	const UGameplayBehaviorSmartObjectBehaviorDefinition* SmartObjectGameplayBehaviorDefinition = SmartObjectSubsystem.MarkSlotAsOccupied<UGameplayBehaviorSmartObjectBehaviorDefinition>(InstanceData->ClaimedHandle);
	const UGameplayBehaviorConfig* GameplayBehaviorConfig = SmartObjectGameplayBehaviorDefinition != nullptr ? SmartObjectGameplayBehaviorDefinition->GameplayBehaviorConfig : nullptr;
	UGameplayBehavior* GameplayBehavior = GameplayBehaviorConfig != nullptr ? GameplayBehaviorConfig->GetBehavior(SmartObjectSubsystem.GetWorldRef()) : nullptr;
	if (GameplayBehavior)
	{
		const USmartObjectComponent* SmartObjectComponent = SmartObjectSubsystem.GetSmartObjectComponent(InstanceData->ClaimedHandle);
		AActor& InteractorActor = Context.GetExternalData(ActorHandle);
		AActor* InteracteeActor = SmartObjectComponent ? SmartObjectComponent->GetOwner() : nullptr;
		bBehaviorActive = UGameplayBehaviorSubsystem::TriggerBehavior(*GameplayBehavior, InteractorActor,GameplayBehaviorConfig, InteracteeActor);
		// Behavior can be successfully triggered AND ended synchronously. We are only interested to register callback when still running
		if (bBehaviorActive)
		{
			InstanceData->GameplayBehavior = GameplayBehavior;
			InstanceData->InteractorActor = &InteractorActor;
			InstanceData->OnBehaviorFinishedNotifyHandle = GameplayBehavior->GetOnBehaviorFinishedDelegate().AddUObject(
				InstanceData, &UJxxUseClaimedSmartObjectTaskInstanceData::OnSmartObjectBehaviorFinished);
		}
	}

	return  bBehaviorActive ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

bool FJxxUseClaimedSmartObjectTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(SmartObjectSubsystemHandle);
	Linker.LinkExternalData(ActorHandle);

	return true;
}

void FJxxUseClaimedSmartObjectTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	USmartObjectSubsystem& SmartObjectSubsystem = Context.GetExternalData(SmartObjectSubsystemHandle);
	AActor& InteractorActor = Context.GetExternalData(ActorHandle);

	if (!InstanceData->bBehaviorIsInterrupted.IsSet())
	{
		if (InstanceData->GameplayBehavior)
		{
			InstanceData->GameplayBehavior->GetOnBehaviorFinishedDelegate().Remove(InstanceData->OnBehaviorFinishedNotifyHandle);
			InstanceData->GameplayBehavior->AbortBehavior(InteractorActor);
		}
	}


	//不需要在这里进行release,因为这个task的父级必然会有FindAndClaim task,会在执行完毕后由父级task负责Release handle.
	/*if (InstanceData->ClaimedHandle.IsValid())
	{
		SmartObjectSubsystem.Release(InstanceData->ClaimedHandle);
		InstanceData->ClaimedHandle.Invalidate();
	}*/

}

EStateTreeRunStatus FJxxUseClaimedSmartObjectTask::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	if (!InstanceData->bBehaviorIsInterrupted.IsSet())
	{
		return EStateTreeRunStatus::Running;
	}

	return InstanceData->bBehaviorIsInterrupted.GetValue() ? EStateTreeRunStatus::Failed : EStateTreeRunStatus::Succeeded;
}

