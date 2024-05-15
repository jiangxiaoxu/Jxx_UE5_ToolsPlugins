// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayMontageNotify_Ext.h"


UE_DEFINE_GAMEPLAY_TAG_STATIC(MontageNotify, "MontageNotify");

UAnimNotify_PlayMontageNotify_Ext::UAnimNotify_PlayMontageNotify_Ext()
{
	bIsNativeBranchingPoint = true;


#if WITH_EDITORONLY_DATA
	NotifyColor = FLinearColor(0.64, 0.9, 0.5).ToFColor(true);
#endif
}

void UAnimNotify_PlayMontageNotify_Ext::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->OnPlayMontageNotifyBegin.Broadcast(GetMyNotifyName(), BranchingPointPayload);
		}
	}
}

FString UAnimNotify_PlayMontageNotify_Ext::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("%s    "), *GetMyNotifyName().ToString());
}

#if WITH_EDITOR
bool UAnimNotify_PlayMontageNotify_Ext::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif // WITH_EDITOR




//----------------------------------------------------------------------------------------


UAnimNotify_PlayMontageNotifyWindow_Ext::UAnimNotify_PlayMontageNotifyWindow_Ext()
{
	bIsNativeBranchingPoint = true;

	

#if WITH_EDITORONLY_DATA
	NotifyColor = FLinearColor(0.64, 0.7, 0.5).ToFColor(true);
#endif
}

void UAnimNotify_PlayMontageNotifyWindow_Ext::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->OnPlayMontageNotifyBegin.Broadcast(GetMyNotifyName(), BranchingPointPayload);
		}
	}
}

void UAnimNotify_PlayMontageNotifyWindow_Ext::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->OnPlayMontageNotifyEnd.Broadcast(GetMyNotifyName(), BranchingPointPayload);
		}
	}
}

FString UAnimNotify_PlayMontageNotifyWindow_Ext::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("%s    "), *GetMyNotifyName().ToString());
}

#if WITH_EDITOR
bool UAnimNotify_PlayMontageNotifyWindow_Ext::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return Super::CanBePlaced(Animation);
}
#endif // WITH_EDITOR



