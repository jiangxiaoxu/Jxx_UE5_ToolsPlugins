// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxCanActivateAbilityCondition.h"

#include "AbilitySystemGlobals.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"


bool FJxxCanActivateAbilityCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	APawn* Pawn = Context.GetExternalDataPtr(PawnHandle);
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);

	if (!ASC || !InstanceData.AbilityClass)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = ASC->AbilityActorInfo.Get();
	if (!ActorInfo)
	{
		return false;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(InstanceData.AbilityClass);
	if (!Spec)
	{
		return false;
	}

	FGameplayTagContainer FailureTags;

	const bool bCanActivate = Spec->Ability->CanActivateAbility(Spec->Handle, ActorInfo, nullptr, nullptr, &FailureTags);
	const bool bShouldAbilityRespondToEvent = Spec->Ability->ShouldAbilityRespondToEvent(ActorInfo, &InstanceData.TestEventData);

	return (bCanActivate && bShouldAbilityRespondToEvent) ^ bInvert;
}

bool FJxxCanActivateAbilityCondition::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(PawnHandle);
	return true;
}