// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeConditionBase.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "JxxCanActivateAbilityCondition.generated.h"


USTRUCT()
struct JXXAI_API FJxxCanActivateAbilityConditionInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	TSubclassOf<class UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bTestRespondToEvent;

	UPROPERTY(EditAnywhere, Category = Parameter,meta=(EditCondition="bTestRespondToEvent"))
	FGameplayEventData TestEventData;
};


USTRUCT(DisplayName="Can Activate Ability", Category="Ability")
struct JXXAI_API FJxxCanActivateAbilityCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FJxxCanActivateAbilityConditionInstanceData;

	FJxxCanActivateAbilityCondition() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	virtual bool Link(FStateTreeLinker& Linker) override;

	TStateTreeExternalDataHandle<APawn> PawnHandle;

	UPROPERTY(EditAnywhere, Category = Condition)
	bool bInvert = false;
	
};
