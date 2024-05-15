// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_PlayMontageNotify_Ext.generated.h"




/**
 * 
 */
UCLASS(editinlinenew, const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Montage Notify Ext"))
class JXXEXTENSIONS_API UAnimNotify_PlayMontageNotify_Ext : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotify_PlayMontageNotify_Ext();

	 virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	 virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	 virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;

#endif
protected:

	// Name of notify that is passed to the PlayMontage K2Node.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify", meta = (Categories = "MontageNotify"))
	FGameplayTag NotifyTag;

	FORCEINLINE FName GetMyNotifyName()const
	{
		return NotifyTag.GetTagName();
	}

};



UCLASS(editinlinenew, const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Montage Notify Window Ext"))
class JXXEXTENSIONS_API UAnimNotify_PlayMontageNotifyWindow_Ext : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UAnimNotify_PlayMontageNotifyWindow_Ext();

	 virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	 virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	 virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	 virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif
protected:

	// Name of notify that is passed to the PlayMontage K2Node.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify", meta = (Categories = "MontageNotify"))
	FGameplayTag NotifyTag;

	FORCEINLINE FName GetMyNotifyName()const
	{
		return NotifyTag.GetTagName();
	}
};