// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ActiveGameplayEffectHandle.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

#include "JxxAbilityExtensionsBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class JXXEXTENSIONS_API UJxxAbilityExtensionsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//获取传入的Handles中,等级是多少.
	UFUNCTION(BlueprintCallable, Category = "Ability|GameplayEffect", Meta = (ReturnDisplayName = "MaxLevel"))
	static float GetActiveGameplayEffectMaxLevel(UAbilitySystemComponent* ASC, TArray<FActiveGameplayEffectHandle> Handles, bool bEnforceOnGoingCheck = true);

	UFUNCTION(BlueprintCallable, Category = "Ability|GameplayEffect", Meta = (ReturnDisplayName = "Level"))
	static float GetActiveGameplayEffectLevel(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle Handle, bool bEnforceOnGoingCheck = true);

	UFUNCTION(BlueprintCallable, Category = "Ability|GameplayEffect")
	static bool IsActiveGameplayEffectExistOnTarget(class UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static bool IsAbilityActiveOnTarget_ByClass(class UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> AbilityClass, bool bIncludeChildClass /*= true*/);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static bool IsAbilityActiveOnTarget_ByAbilityTags(class UAbilitySystemComponent* ASC, FGameplayTagContainer AnyAbilityTags);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static bool IsAbilityActiveOnTarget_ByAbilityTagQuery(class UAbilitySystemComponent* ASC, FGameplayTagQuery AbilityTagQuery);



	UFUNCTION(BlueprintCallable, Category = Ability)
	static void CancelAbilitiesOnTargetByQuery(class UAbilitySystemComponent* ASC, FGameplayTagQuery CancelAbilitiesTagQuery);

	//UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (DefaultToSelf = "Actor"))
	static void RemoveAbility(AActor* Actor, TSubclassOf<class UGameplayAbility> AbilityClass, bool bWaitAbilityEnd);

	UFUNCTION(BlueprintPure)
	static bool WasSuccessfullyApplied(FActiveGameplayEffectHandle Handle);

	/** Tests if all modifiers in this GameplayEffect will leave the attribute > 0.f */
	UFUNCTION(BlueprintCallable, Meta = (ReturnDisplayName = "bCan"))
	static bool CanApplyAttributeModifiers(UAbilitySystemComponent* ASC, FGameplayEffectSpec TestSpec);

	UFUNCTION(BlueprintCallable, Category = Ability)
	static void CancelAbilitiesOnTarget(UAbilitySystemComponent* ASC, FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTagss);

	UFUNCTION(BlueprintCallable)
	static void GetDebugGameplayEffectStateInfo(class UAbilitySystemComponent* InASC, FGameplayTag ParentTag, TArray<FString>& OutDebugString, TArray<FActiveGameplayEffectHandle>& OutHandles);


	UFUNCTION(BlueprintCallable)
	static bool GetEffectContext_Copy(FGameplayEffectSpecHandle InEffectSpec, FGameplayEffectContextHandle& OutEffectContext);

	UFUNCTION(BlueprintCallable)
	static void GetAllGrantedTags(FGameplayEffectSpecHandle InEffectSpec, FGameplayTagContainer& Container);

	UFUNCTION(BlueprintCallable)
	static void GetAllAssetTags(FGameplayEffectSpecHandle InEffectSpec, FGameplayTagContainer& Container);


	UFUNCTION(BlueprintCallable)
	static  bool GetAbilityActorInfo(UAbilitySystemComponent* InASC, FGameplayAbilityActorInfo& OutActorInfo);


	UFUNCTION(BlueprintCallable)
	static FGameplayAbilityTargetDataHandle MakeAbilityTargetData_ByHits(const TArray<FHitResult>& HitResults);

	UFUNCTION(BlueprintCallable, meta = (DevelopmentOnly))
	static void SetIgnoreAbilitySystemCooldowns(bool bIsIgnore);

	UFUNCTION(BlueprintCallable, meta = (DevelopmentOnly))
	static void SetIgnoreAbilitySystemCosts(bool bIsIgnore);

	UFUNCTION(BlueprintCallable)
	static bool HasAnySuccessfulApplied(const TArray<FActiveGameplayEffectHandle>& Handles);

	UFUNCTION(BlueprintCallable, Meta = (ReturnDisplayName = "bCan"))
	static bool CanActivateAbility(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> AbilityClass, FGameplayTagContainer& FailureTags);

	UFUNCTION(BlueprintPure)
	static float GetDuration(FGameplayEffectSpecHandle InEffectSpecHandle);


	UFUNCTION(BlueprintCallable)
	static float GetSetByCallerMagnitude(FGameplayEffectSpecHandle InEffectSpecHandle, FGameplayTag DataTag, bool WarnIfNotFound = true, float DefaultIfNotFound = 0.f);

	UFUNCTION(BlueprintCallable, Category = "GameplayTag")
	static bool DoesGameplayTagContainerMeetTagRequirements(const FGameplayTagContainer& Container, const FGameplayTagRequirements& Requirements);

	UFUNCTION(BlueprintCallable, Category = "GameplayTag", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool DoesGameplayTagContainerMeetTagRequirements_Exec(const FGameplayTagContainer& Container, const FGameplayTagRequirements& Requirements);

	UFUNCTION(BlueprintCallable, Category = "GameplayEffect", DisplayName = "IsValid")
	static bool IsValid_GameplayEffectSpecHandle(FGameplayEffectSpecHandle InHandle);

	UFUNCTION(BlueprintCallable, Category = "GameplayEffect", DisplayName = "IsValid_Exec", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsValid_GameplayEffectSpecHandle_Exec(FGameplayEffectSpecHandle InHandle);


	UFUNCTION(BlueprintPure)
	static UAnimInstance* GetAnimInstance(const FGameplayAbilityActorInfo& ActorInfo)
	{
		return ActorInfo.GetAnimInstance();
	}

};
