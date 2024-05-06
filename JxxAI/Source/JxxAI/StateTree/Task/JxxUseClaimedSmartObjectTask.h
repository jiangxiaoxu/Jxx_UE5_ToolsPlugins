// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "SmartObjectSubsystem.h"
#include "JxxUseClaimedSmartObjectTask.generated.h"



UCLASS()
class JXXAI_API UJxxUseClaimedSmartObjectTaskInstanceData:public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Input)
	FSmartObjectClaimHandle ClaimedHandle;

	UPROPERTY()
	TObjectPtr<class UGameplayBehavior> GameplayBehavior;

	void OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted);

	FDelegateHandle OnBehaviorFinishedNotifyHandle;

	TOptional<bool> bBehaviorIsInterrupted;
	
	UPROPERTY()
	TObjectPtr<AActor> InteractorActor;
};


USTRUCT(meta = (DisplayName = "Use Claimed Smart Object"), Category = "Smart Object")
struct JXXAI_API FJxxUseClaimedSmartObjectTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UJxxUseClaimedSmartObjectTaskInstanceData;

	FJxxUseClaimedSmartObjectTask();

	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;


	TStateTreeExternalDataHandle<USmartObjectSubsystem> SmartObjectSubsystemHandle;
	TStateTreeExternalDataHandle<AActor> ActorHandle;
};
