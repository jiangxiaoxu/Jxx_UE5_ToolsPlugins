// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "InstancedStruct.h"
#include "JxxActivateGameplayAbilityTaskWithPayload.generated.h"



UCLASS()
class JXXAI_API UJxxActivateGameplayAbilityWithPayloadTaskInstanceData :public UObject
{
	GENERATED_BODY()

public:

	UJxxActivateGameplayAbilityWithPayloadTaskInstanceData();

	UPROPERTY(EditAnywhere, Category = Parameter)
	TSubclassOf< UGameplayAbility> AbilityClass;

	//会作为Payload的EventTag被传入.
	UPROPERTY(EditAnywhere, Category = Parameter)
	FGameplayTag ActivateEventTag;

	// 会存于EventData.OptionalObject2中,在Ability中,将CurrrentEventData结构体连接到方法GetInstancedStructValue来获取,会自动生成提取节点.
	UPROPERTY(EditAnywhere, Category = Parameter)
	FInstancedStruct PayloadStructData;


	UPROPERTY(EditAnywhere, Category = Parameter)
	AActor* Target;

	UPROPERTY(EditAnywhere, Category = Parameter)
	AActor* Instigator;

	//可选的Payload object类型对象.
	UPROPERTY(EditAnywhere, Category = Parameter)
	UObject* PayloadOptionalObject1;

	//可选的Payload float类型对象.
	UPROPERTY(EditAnywhere, Category = Parameter)
	float PayloadEventMagnitude;

	//为True时,Ability的结束类型为Cancelled的情况下,这个StateTreeTask会视为Failed.
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bTreatAbilityCancelledAsFailed;

	//为True时,如果StateTreeTask先于Ability结束,则会一并结束掉已激活的Ability,确保Ability的生命周期与StateTreeTask一致.
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bCancelAbilityWhenTaskExit;

	void OnAbilityFailedToActivate(const UGameplayAbility* Ability, const struct FGameplayTagContainer& FailureReason);

	void OnAbilityEnded(const  FAbilityEndedData& EndedData);

	void Cleanup();


	TOptional<FGameplayTagContainer>  AbilityFailedReason;

	TOptional<bool> AbilityEndIsCancelled;

	UPROPERTY()
	TObjectPtr< UAbilitySystemComponent> CacheASC;

	FGameplayAbilitySpecHandle CacheAbilitySpecHandle;

	FDelegateHandle OnAbilityEndedHandle;

	FDelegateHandle OnAbilityFailedToActivateHandle;
};
/**
 * 
 */
USTRUCT(meta = (DisplayName = "Activate Ability"), Category = "Ability System")
struct JXXAI_API FJxxActivateGameplayAbilityTaskWithPayload : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UJxxActivateGameplayAbilityWithPayloadTaskInstanceData;

	FJxxActivateGameplayAbilityTaskWithPayload()
	{
	}

	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	//virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const override;

	TStateTreeExternalDataHandle<APawn> PawnHandle;

};