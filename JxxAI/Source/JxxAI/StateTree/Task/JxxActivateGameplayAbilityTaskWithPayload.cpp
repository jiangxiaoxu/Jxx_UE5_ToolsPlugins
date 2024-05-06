// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxActivateGameplayAbilityTaskWithPayload.h"

#include "AbilitySystemGlobals.h"
#include "InstancedStructWrapperObject.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Activate_WithPayload, "Ability.Activate.WithPayload");

UJxxActivateGameplayAbilityWithPayloadTaskInstanceData::UJxxActivateGameplayAbilityWithPayloadTaskInstanceData()
{
	ActivateEventTag = TAG_Ability_Activate_WithPayload;

	bTreatAbilityCancelledAsFailed = false;
	bCancelAbilityWhenTaskExit = true;
}

void UJxxActivateGameplayAbilityWithPayloadTaskInstanceData::OnAbilityFailedToActivate(const UGameplayAbility* Ability,
                                                                                         const FGameplayTagContainer& FailureReason)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: AbilityFailedToActivate: %s ."),
					       ANSI_TO_TCHAR(__FUNCTION__),*GetNameSafe(Ability->GetClass()));
}

void UJxxActivateGameplayAbilityWithPayloadTaskInstanceData::OnAbilityEnded(const FAbilityEndedData& EndedData)
{
	if (EndedData.AbilitySpecHandle == CacheAbilitySpecHandle)
	{
		AbilityEndIsCancelled = EndedData.bWasCancelled;

		if (ensure(CacheASC))
		{
			CacheASC->OnAbilityEnded.Remove(OnAbilityEndedHandle);
		}
	}
}

void UJxxActivateGameplayAbilityWithPayloadTaskInstanceData::Cleanup()
{
	AbilityFailedReason.Reset();
	AbilityEndIsCancelled.Reset();
	CacheASC = nullptr;
	CacheAbilitySpecHandle = {};
	OnAbilityEndedHandle = {};
	OnAbilityFailedToActivateHandle = {};
}

bool FJxxActivateGameplayAbilityTaskWithPayload::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(PawnHandle);
	return true;
}

EStateTreeRunStatus FJxxActivateGameplayAbilityTaskWithPayload::EnterState(FStateTreeExecutionContext& Context,
                                                                  const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	APawn* Pawn = Context.GetExternalDataPtr(PawnHandle);
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);

	if (!ASC || !InstanceData->AbilityClass)
	{
		return EStateTreeRunStatus::Failed;
	}

	FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(InstanceData->AbilityClass);
	if (!AbilitySpec)
	{
		return EStateTreeRunStatus::Failed;
	}

	InstanceData->CacheASC = ASC;
	InstanceData->CacheAbilitySpecHandle = AbilitySpec->Handle;
	InstanceData->OnAbilityEndedHandle = ASC->OnAbilityEnded.AddUObject(
		InstanceData, &UJxxActivateGameplayAbilityWithPayloadTaskInstanceData::OnAbilityEnded);

	InstanceData->OnAbilityFailedToActivateHandle = ASC->AbilityFailedCallbacks.AddUObject(
		InstanceData, &UJxxActivateGameplayAbilityWithPayloadTaskInstanceData::OnAbilityFailedToActivate);

	FGameplayEventData Payload;
	UInstancedStructWrapperObject* WrapperObject=NewObject<UInstancedStructWrapperObject>(GetTransientPackage(), NAME_None);
	WrapperObject->InstancedStruct = InstanceData->PayloadStructData;


	Payload.Target = InstanceData->Target;
	Payload.Instigator = InstanceData->Instigator;
	Payload.OptionalObject = InstanceData->PayloadOptionalObject1;
	Payload.OptionalObject2 = WrapperObject;
	Payload.EventMagnitude = InstanceData->PayloadEventMagnitude;

	const bool bResult = ASC->TriggerAbilityFromGameplayEvent(AbilitySpec->Handle, ASC->AbilityActorInfo.Get(), InstanceData->ActivateEventTag, &Payload, *ASC);
	if (!bResult)
	{
		return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FJxxActivateGameplayAbilityTaskWithPayload::Tick(FStateTreeExecutionContext& Context,const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	EStateTreeRunStatus ResultStatus = EStateTreeRunStatus::Running;

	if (InstanceData->AbilityEndIsCancelled.IsSet())
	{
		const bool bAbilityEndIsCancelled = InstanceData->AbilityEndIsCancelled.GetValue();
		ResultStatus = (bAbilityEndIsCancelled && InstanceData->bTreatAbilityCancelledAsFailed) ? EStateTreeRunStatus::Failed : EStateTreeRunStatus::Succeeded;
	}
	return ResultStatus;
}

void FJxxActivateGameplayAbilityTaskWithPayload::ExitState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	if (InstanceData->CacheASC && InstanceData->AbilityClass)
	{
		InstanceData->CacheASC->OnAbilityEnded.Remove(InstanceData->OnAbilityEndedHandle);
		InstanceData->CacheASC->AbilityFailedCallbacks.Remove(InstanceData->OnAbilityFailedToActivateHandle);

		if (false == InstanceData->AbilityEndIsCancelled.IsSet())
		{
			if (InstanceData->bCancelAbilityWhenTaskExit)
			{
				InstanceData->CacheASC->CancelAbility(InstanceData->AbilityClass.GetDefaultObject());
				if (FGameplayAbilitySpec* Spec = InstanceData->CacheASC->FindAbilitySpecFromClass(InstanceData->AbilityClass))
				{
					if (Spec->IsActive())
					{
						UE_LOG(LogTemp, Error, TEXT("%s: Try CancelAbility: %s But It Still Active? Sth Wrong ???"),
						       ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(InstanceData->AbilityClass));
					}
				}
			}
		}
	}

	InstanceData->Cleanup();
}


