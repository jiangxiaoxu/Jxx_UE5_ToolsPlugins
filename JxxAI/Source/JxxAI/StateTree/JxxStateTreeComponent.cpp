// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxStateTreeComponent.h"
#include "GameplayTasksComponent.h"
#include "StateTreeExecutionContext.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "Components/StateTreeComponentSchema.h"
#include "Engine/World.h"
#include "Tasks/AITask.h"

#define STATETREE_LOG(Verbosity, Format, ...) UE_VLOG(GetOwner(), LogStateTree, Verbosity, Format, ##__VA_ARGS__)
#define STATETREE_CLOG(Condition, Verbosity, Format, ...) UE_CVLOG((Condition), GetOwner(), LogStateTree, Verbosity, Format, ##__VA_ARGS__)

void UJxxStateTreeComponent::SetStateTreeReference(const FJxxStateTreeReference& InRef,bool bShouldStartLogic)
{
	if (IsRunning() || IsPaused())
	{
		StopLogic(TEXT("Update StateTree"));
	}

	StateTreeRef = InRef.StateTreeRef;

	{
		if (!StateTreeRef.IsValid())
		{
			STATETREE_LOG(Error, TEXT("%s: StateTree asset is not set, cannot initialize."),ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}

		const FStateTreeExecutionContext Context(*GetOwner(), *StateTreeRef.GetStateTree(), InstanceData);
		if (!Context.IsValid())
		{
			STATETREE_LOG(Error, TEXT("%s: Failed to init StateTreeContext."), ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
	}

	if (bShouldStartLogic)
	{
		StartLogic();
	}
}

void UJxxStateTreeComponent::InitializeComponent()
{
	if (StateTreeRef.IsValid()) //只有当被提前赋予StateTreeRef才需要执行检测.
	{
		Super::InitializeComponent();
	}
}

void UJxxStateTreeComponent::BeginPlay()
{
	Super::Super::BeginPlay();

	if (StateTreeRef.IsValid()) //只有当被提前赋予StateTreeRef才需要执行检测.
	{
		if (AIOwner == nullptr && bStartLogicAutomatically)
		{
			StartLogic();
		}
	}
}

