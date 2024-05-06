// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "JxxStateTreeComponent.generated.h"


USTRUCT(BlueprintType)
struct FJxxStateTreeReference
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = AI, meta=(Schema="/Script/GameplayStateTreeModule.StateTreeComponentSchema"))
	FStateTreeReference StateTreeRef;

	bool IsValid()const
	{
		return StateTreeRef.IsValid();
	}
	
};

/*
 * 相比于原版,增加了动态性 现在可以运行时设置StateTreeReference.
 */

UCLASS(ClassGroup = AI, HideCategories = (Activation, Collision), meta = (BlueprintSpawnableComponent))
class JXXAI_API UJxxStateTreeComponent : public UStateTreeComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetStateTreeReference(const FJxxStateTreeReference& InRef,bool bShouldStartLogic=true);

	// BEGIN UActorComponent overrides
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	// END UActorComponent overrides
	
};
