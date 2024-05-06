// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AITypes.h"
#include "GameplayTaskOwnerInterface.h"
#include "StateTreeTaskBase.h"
#include "JxxAIMoveToTask.generated.h"


namespace EPathFollowingRequestResult {	enum Type : int; }
namespace EPathFollowingResult { enum Type : int; }
namespace EPathFollowingStatus { enum Type : int; }

enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

class UAITask_MoveTo;
class AAIController;
class UStateTreeComponent;


UCLASS()
class JXXAI_API UJxxAIMoveToTaskInstanceData:public UObject, public IGameplayTaskOwnerInterface
{
	GENERATED_BODY()

public:
	UJxxAIMoveToTaskInstanceData();


	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bUseGoalActor;

	UPROPERTY(EditAnywhere, Category = Parameter,meta = (EditConditionHides,EditCondition="bUseGoalActor"))
	TObjectPtr<AActor> GoalActor;

	UPROPERTY(EditAnywhere, Category = Parameter,meta = (EditConditionHides,EditCondition="!bUseGoalActor"))
	FVector GoalLocation;

	/** fixed distance added to threshold between AI and goal location in destination reach test */
	UPROPERTY( EditAnywhere,Category = Parameter, meta=(ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius;

	/** "None" will result in default filter being used */
	UPROPERTY(EditAnywhere, Category = Parameter)
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	/** if task is expected to react to changes to location represented by BB key 
	 *	this property can be used to tweak sensitivity of the mechanism. Value is 
	 *	recommended to be less than AcceptableRadius */
	//UPROPERTY(EditAnywhere, Category = Parameter, meta = (ClampMin = "1", UIMin = "1", EditCondition="bObserveBlackboardValue", DisplayAfter="bObserveBlackboardValue"))
	//float ObservedBlackboardValueTolerance;

	// /** if move goal in BB changes the move will be redirected to new location */
	// UPROPERTY(EditAnywhere, Category = Parameter)
	// uint32 bObserveBlackboardValue : 1;

	/*
	 启用之后UPathFollowingComponent::UpdateMoveFocus()
	 EAIFocusPriority::Move 类型的FocalPoint会被设置为移动目标点,
	 不器用的话Move 类型的FocalPoint会被设置为当前角色移动的前方.
	 */
	UPROPERTY(EditAnywhere, Category = Parameter)
	uint32 bAllowStrafe : 1;

	/** if set, use incomplete path when goal can't be reached */
	UPROPERTY(EditAnywhere, Category = Parameter, AdvancedDisplay)
	uint32 bAllowPartialPath : 1;

	/** if set, path to goal actor will update itself when actor moves */
	UPROPERTY(EditAnywhere, Category = Parameter, AdvancedDisplay)
	uint32 bTrackMovingGoal : 1;

	/** if set, goal location will be projected on navigation data (navmesh) before using */
	UPROPERTY(EditAnywhere, Category = Parameter, AdvancedDisplay)
	uint32 bProjectGoalLocation : 1;

	/** if set, radius of AI's capsule will be added to threshold between AI and goal location in destination reach test  */
	UPROPERTY(EditAnywhere, Category = Parameter)
	uint32 bReachTestIncludesAgentRadius : 1;
	
	/** if set, radius of goal's capsule will be added to threshold between AI and goal location in destination reach test  */
	UPROPERTY(EditAnywhere, Category = Parameter)
	uint32 bReachTestIncludesGoalRadius : 1;


	// my transient 
	TWeakObjectPtr<class UAITask_MoveTo> Task;

	TWeakObjectPtr<class UStateTreeComponent> CachedStateTreeComp;

	TOptional<TEnumAsByte<EPathFollowingResult::Type>> MoveResultOpt;

	//uint8 bObserverCanFinishTask : 1;

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task)override;
	virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override;
	virtual UGameplayTasksComponent* GetGameplayTasksComponent(const UGameplayTask& Task)const override;


};


/*
如果需要面朝一个特定方向移动的话,不需要使用bAllowStrafe,
需要并用AISetScopeFocus 的task.
CthuCharacterMoveComp中会使用EAIFocusPriority::Gameplay的目标点当被设置的情况下作为面朝方向.
 */
USTRUCT(meta = (DisplayName = "AI MoveTo"),Category="AI")
struct JXXAI_API FJxxAIMoveToTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UJxxAIMoveToTaskInstanceData;


	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	//virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus,const FStateTreeActiveStates& CompletedActiveStates) const override;

	EStateTreeRunStatus PerformMoveTask(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)const;

	/** prepares move task for activation */
	UAITask_MoveTo* PrepareMoveTask(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest)const;
	

	/** Handle to retrieve AIController. */
	TStateTreeExternalDataHandle<AAIController> AIControllerHandle;

	TStateTreeExternalDataHandle<UStateTreeComponent> StateTreeCompHandle;

};
