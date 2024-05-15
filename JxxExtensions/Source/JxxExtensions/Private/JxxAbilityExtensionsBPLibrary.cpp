// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxAbilityExtensionsBPLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"


float UJxxAbilityExtensionsBPLibrary::GetActiveGameplayEffectMaxLevel(UAbilitySystemComponent* ASC,TArray<FActiveGameplayEffectHandle> Handles,
                                                                      bool bEnforceOnGoingCheck)
{
	float OutLevel = 0;
	if (IsValid(ASC))
	{
		for (FActiveGameplayEffectHandle Handle : Handles)
		{
			const FActiveGameplayEffect* ActiveGameplayEffect = ASC->GetActiveGameplayEffect(Handle);
			if (ActiveGameplayEffect)
			{
				if (!ActiveGameplayEffect->bIsInhibited || !bEnforceOnGoingCheck)
				{
					float TempLevel = ActiveGameplayEffect->Spec.GetLevel();
					OutLevel = FMath::Max(OutLevel, TempLevel);
				}
			}
		}
	}

	return OutLevel;
}

float UJxxAbilityExtensionsBPLibrary::GetActiveGameplayEffectLevel(UAbilitySystemComponent* ASC,FActiveGameplayEffectHandle Handle,bool bEnforceOnGoingCheck)
{
	float OutLevel = 0;
	if (IsValid(ASC))
	{
			const FActiveGameplayEffect* ActiveGameplayEffect = ASC->GetActiveGameplayEffect(Handle);
			if (ActiveGameplayEffect)
			{
				if (!ActiveGameplayEffect->bIsInhibited || !bEnforceOnGoingCheck)
				{
					OutLevel = ActiveGameplayEffect->Spec.GetLevel();
				}
			}
	}
	return OutLevel;
}

bool UJxxAbilityExtensionsBPLibrary::IsActiveGameplayEffectExistOnTarget(UAbilitySystemComponent* ASC,FActiveGameplayEffectHandle Handle)
{
	if (!IsValid(ASC))
	{
		return false;
	}

	return ASC->GetActiveGameplayEffect(Handle) != nullptr;
}

bool UJxxAbilityExtensionsBPLibrary::IsAbilityActiveOnTarget_ByClass(UAbilitySystemComponent* ASC,TSubclassOf<UGameplayAbility> AbilityClass,
	bool bIncludeChildClass)
{
	if (!IsValid(ASC) || !AbilityClass)
	{
		return false;
	}

	FScopedAbilityListLock ActiveScopeLock(*ASC);
	const TArray<FGameplayAbilitySpec>& SpecArray = ASC->GetActivatableAbilities();
	auto FoundPtr = SpecArray.FindByPredicate(
		[AbilityClass, bIncludeChildClass](const FGameplayAbilitySpec& TestSpec)
		{
			if (TestSpec.Ability && TestSpec.IsActive())
			{
				if (bIncludeChildClass)
				{
					return TestSpec.Ability->GetClass()->IsChildOf(AbilityClass);
				}
				else
				{
					return TestSpec.Ability == AbilityClass.GetDefaultObject();
				}
			}
			else
			{
				return false;
			}
		});

	return FoundPtr != nullptr;
}

bool UJxxAbilityExtensionsBPLibrary::IsAbilityActiveOnTarget_ByAbilityTags(UAbilitySystemComponent* ASC,FGameplayTagContainer AnyAbilityTags)
{
	if (!IsValid(ASC) || !AnyAbilityTags.IsValid())
	{
		return false;
	}

	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasAny(AnyAbilityTags))
		{
			if (Spec.IsActive())
			{
				return true;
			}
		}
	}

	return false;
}

bool UJxxAbilityExtensionsBPLibrary::IsAbilityActiveOnTarget_ByAbilityTagQuery(UAbilitySystemComponent* ASC,FGameplayTagQuery AbilityTagQuery)
{
	if (!IsValid(ASC) || AbilityTagQuery.IsEmpty())
	{
		return false;
	}

	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.IsActive() && AbilityTagQuery.Matches(Spec.Ability->AbilityTags))
		{
			return true;
		}
	}

	return false;
}

void UJxxAbilityExtensionsBPLibrary::CancelAbilitiesOnTargetByQuery(UAbilitySystemComponent* ASC,FGameplayTagQuery CancelAbilitiesTagQuery)
{
	if (!IsValid(ASC) || CancelAbilitiesTagQuery.IsEmpty())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLock(*ASC);

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.IsActive() && CancelAbilitiesTagQuery.Matches(Spec.Ability->AbilityTags))
		{
			AbilitySpecHandles.Add(Spec.Handle);
		}
	}

	for (FGameplayAbilitySpecHandle Handle : AbilitySpecHandles)
	{
		ASC->CancelAbilityHandle(Handle);
	}

}

void UJxxAbilityExtensionsBPLibrary::RemoveAbility(AActor* Actor,TSubclassOf<UGameplayAbility> AbilityClass,bool bWaitAbilityEnd)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (IsValid(ASC) && AbilityClass)
	{
		if (FGameplayAbilitySpec* SpecPtr = ASC->FindAbilitySpecFromClass(AbilityClass))
		{
			if (bWaitAbilityEnd)
			{
				ASC->SetRemoveAbilityOnEnd(SpecPtr->Handle);
			}
			else
			{
				ASC->ClearAbility(SpecPtr->Handle);
			}
		}
	}
}

bool UJxxAbilityExtensionsBPLibrary::WasSuccessfullyApplied(FActiveGameplayEffectHandle Handle)
{
	return Handle.WasSuccessfullyApplied();
}

bool UJxxAbilityExtensionsBPLibrary::CanApplyAttributeModifiers(UAbilitySystemComponent* ASC,FGameplayEffectSpec TestSpec)
{
	if (!IsValid(ASC) || TestSpec.Def == nullptr)
	{
		return false;
	}
	return ASC->CanApplyAttributeModifiers(TestSpec.Def, TestSpec.GetLevel(), TestSpec.GetEffectContext());
}

void UJxxAbilityExtensionsBPLibrary::CancelAbilitiesOnTarget(UAbilitySystemComponent* ASC,FGameplayTagContainer WithTags,FGameplayTagContainer WithoutTags)
{
	if (IsValid(ASC) && (WithTags.IsValid()))
	{
		ASC->CancelAbilities(&WithTags, WithoutTags.IsValid() ? &WithoutTags : nullptr);
	}
}

void UJxxAbilityExtensionsBPLibrary::GetDebugGameplayEffectStateInfo(UAbilitySystemComponent* InASC,FGameplayTag ParentTag,TArray<FString>& OutDebugString,
	TArray<FActiveGameplayEffectHandle>& OutHandles)
{
	if (!IsValid(InASC))
	{
		return;
	}

	if (!ParentTag.IsValid())
	{
		ParentTag = FGameplayTag::RequestGameplayTag("state");
	}

	FGameplayEffectQuery Query;
	Query.CustomMatchDelegate.BindLambda(
		[&ParentTag](const FActiveGameplayEffect& Effect)->bool
		{
			FGameplayTagContainer GrantedTags;
			Effect.Spec.GetAllGrantedTags(GrantedTags);

			return GrantedTags.HasTag(ParentTag);
		});

	TArray<FActiveGameplayEffectHandle> Handles = InASC->GetActiveEffects(Query);


	TArray<FString> DebugStringArray;

	for (FActiveGameplayEffectHandle Handle : Handles)
	{
		const FActiveGameplayEffect* ActiveGE = InASC->GetActiveGameplayEffect(Handle);
		if (ActiveGE)
		{
			FString Str_1 = FString::Printf(TEXT("%s - (Level: %.2f. Stacks: %d)"), *ActiveGE->Spec.Def->GetName(), ActiveGE->Spec.GetLevel(), ActiveGE->Spec.GetStackCount());

			FGameplayTagContainer GrantedTags;
			ActiveGE->Spec.GetAllGrantedTags(GrantedTags);

			FString Str_2 = FString::Printf(TEXT("GrantTag:[ %s ]"), *GrantedTags.ToStringSimple());

			FString Str_3 = FString::Printf(TEXT("TimeRemain %.2f"), ActiveGE->GetTimeRemaining(InASC->GetWorld()->GetTimeSeconds()));


			FString Str = FString::Printf(TEXT("%s %s %s"), *Str_1, *Str_2, *Str_3);
			DebugStringArray.Add(MoveTemp(Str));
		}
	}

	OutDebugString = DebugStringArray;
	OutHandles = Handles;
}

bool UJxxAbilityExtensionsBPLibrary::GetEffectContext_Copy(FGameplayEffectSpecHandle InEffectSpec,FGameplayEffectContextHandle& OutEffectContext)
{
	OutEffectContext = {};

	if (InEffectSpec.IsValid())
	{
		const FGameplayEffectContextHandle& EffectContextHandle = InEffectSpec.Data->GetEffectContext();
		OutEffectContext = EffectContextHandle.Duplicate();
		return true;
	}
	return false;
}

void UJxxAbilityExtensionsBPLibrary::GetAllGrantedTags(FGameplayEffectSpecHandle InEffectSpec,FGameplayTagContainer& Container)
{
	Container = {};

	if (InEffectSpec.IsValid())
	{
		InEffectSpec.Data->GetAllGrantedTags(Container);
	}
}

void UJxxAbilityExtensionsBPLibrary::GetAllAssetTags(FGameplayEffectSpecHandle InEffectSpec,FGameplayTagContainer& Container)
{
	Container = {};

	if (InEffectSpec.IsValid())
	{
		InEffectSpec.Data->GetAllAssetTags(Container);
	}
}

bool UJxxAbilityExtensionsBPLibrary::GetAbilityActorInfo(UAbilitySystemComponent* InASC,FGameplayAbilityActorInfo& OutActorInfo)
{
	OutActorInfo = {};

	if (InASC)
	{
		check(InASC->AbilityActorInfo.IsValid());
		OutActorInfo = *InASC->AbilityActorInfo.Get();
		return true;
	}
	else
	{
		return false;
	}
}

FGameplayAbilityTargetDataHandle UJxxAbilityExtensionsBPLibrary::MakeAbilityTargetData_ByHits(const TArray<FHitResult>& HitResults)
{
	FGameplayAbilityTargetDataHandle TargetDataHandle;

	for (const FHitResult& Hit : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* SingleTargetHit = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
		TargetDataHandle.Add(SingleTargetHit);
	}

	return TargetDataHandle;
}

void UJxxAbilityExtensionsBPLibrary::SetIgnoreAbilitySystemCooldowns(bool bIsIgnore)
{
#if (!(UE_BUILD_SHIPPING || UE_BUILD_TEST))
	static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("AbilitySystem.IgnoreCooldowns"));
	if (CVar)
	{
		CVar->Set(bIsIgnore);
	}
#endif // WITH_ABILITY_CHEATS
}

void UJxxAbilityExtensionsBPLibrary::SetIgnoreAbilitySystemCosts(bool bIsIgnore)
{
#if (!(UE_BUILD_SHIPPING || UE_BUILD_TEST))
	static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("AbilitySystem.IgnoreCosts"));
	if (CVar)
	{
		CVar->Set(bIsIgnore);
	}
#endif // WITH_ABILITY_CHEATS
}

bool UJxxAbilityExtensionsBPLibrary::HasAnySuccessfulApplied(const TArray<FActiveGameplayEffectHandle>& Handles)
{
	bool bResult = false;
	for (const FActiveGameplayEffectHandle& Handle : Handles)
	{
		if (Handle.WasSuccessfullyApplied())
		{
			bResult = true;
			break;
		}
	}
	return bResult;
}

bool UJxxAbilityExtensionsBPLibrary::CanActivateAbility(UAbilitySystemComponent* ASC,TSubclassOf<UGameplayAbility> AbilityClass,
	FGameplayTagContainer& FailureTags)
{
	FailureTags = {};

	if (!IsValid(ASC) || !AbilityClass)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = ASC->AbilityActorInfo.Get();
	if (!ActorInfo)
	{
		return false;
	}

	const FGameplayAbilitySpec* FoundSpecPtr = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (!FoundSpecPtr || !FoundSpecPtr->Ability)
	{
		return false;
	}

	const bool bResult = FoundSpecPtr->Ability->CanActivateAbility(FoundSpecPtr->Handle, ActorInfo, nullptr, nullptr, &FailureTags);

	return bResult;
}

float UJxxAbilityExtensionsBPLibrary::GetDuration(FGameplayEffectSpecHandle InEffectSpecHandle)
{
	if (InEffectSpecHandle.Data.IsValid())
	{
		float Duration = InEffectSpecHandle.Data->GetDuration();
		return Duration;
	}
	else
	{
		return -1;
	}
}

float UJxxAbilityExtensionsBPLibrary::GetSetByCallerMagnitude(FGameplayEffectSpecHandle InEffectSpecHandle,FGameplayTag DataTag,bool WarnIfNotFound,
	float DefaultIfNotFound)
{
	float Magnitude = 0;
	if (InEffectSpecHandle.Data.IsValid())
	{
		Magnitude = InEffectSpecHandle.Data->GetSetByCallerMagnitude(DataTag, WarnIfNotFound, DefaultIfNotFound);
	}
	return Magnitude;
}

bool UJxxAbilityExtensionsBPLibrary::DoesGameplayTagContainerMeetTagRequirements(const FGameplayTagContainer& Container,
	const FGameplayTagRequirements& Requirements)
{
	return Requirements.RequirementsMet(Container);
}

bool UJxxAbilityExtensionsBPLibrary::DoesGameplayTagContainerMeetTagRequirements_Exec(const FGameplayTagContainer& Container,
	const FGameplayTagRequirements& Requirements)
{
	return Requirements.RequirementsMet(Container);
}

bool UJxxAbilityExtensionsBPLibrary::IsValid_GameplayEffectSpecHandle(FGameplayEffectSpecHandle InHandle)
{
	return InHandle.IsValid();
}

bool UJxxAbilityExtensionsBPLibrary::IsValid_GameplayEffectSpecHandle_Exec(FGameplayEffectSpecHandle InHandle)
{
	return InHandle.IsValid();
}
