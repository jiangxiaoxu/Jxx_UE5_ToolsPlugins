// Copyright Epic Games, Inc. All Rights Reserved.

#include "JxxExtensionsBPLibrary.h"

#include "DelayAction.h"
#include "JxxExtensions.h"
#include "Components/ShapeComponent.h"
#include "Engine/CancellableAsyncAction.h"


#if WITH_EDITOR
#include "EditorViewportClient.h"
#endif


class FCooldownDelayAction : public FDelayAction
{
public:

	using FDelayAction::FDelayAction;

	/*
	FCooldownDelayAction(float Duration, const FLatentActionInfo& LatentInfo)
	: FDelayAction(Duration, LatentInfo)
	{
	}*/

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		TimeRemaining -= Response.ElapsedTime();
		Response.DoneIf(TimeRemaining <= 0.0f);
	}
};






UJxxExtensionsBPLibrary::UJxxExtensionsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UJxxExtensionsBPLibrary::JxxExtensionsSampleFunction(float Param)
{
	return -1;
}

void UJxxExtensionsBPLibrary::ExecuteWithCooldown(const UObject* WorldContextObject,float Cooldown,FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		FCooldownDelayAction* Action = LatentActionManager.FindExistingAction<FCooldownDelayAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);
		if (Action == nullptr)
		{
			if (LatentInfo.UUID != INDEX_NONE)
			{
				if (UObject* CallbackTarget = LatentInfo.CallbackTarget.Get())
				{
					if (UFunction* ExecutionFunction = CallbackTarget->FindFunction(LatentInfo.ExecutionFunction))
					{
						CallbackTarget->ProcessEvent(ExecutionFunction, &(LatentInfo.Linkage));
					}
					else
					{
						UE_LOG(LogScript, Warning, TEXT("FLatentActionManager::ProcessLatentActions: Could not find latent action resume point named '%s' on '%s' called by '%s'"),
							*LatentInfo.ExecutionFunction.ToString(), *(CallbackTarget->GetPathName()), *(CallbackTarget->GetPathName()));
					}
				}
				else
				{
					UE_LOG(LogScript, Warning, TEXT("FLatentActionManager::ProcessLatentActions: CallbackTarget is None."));
				}
			}

			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FCooldownDelayAction(Cooldown, LatentInfo));
		}
		else
		{
			//still block exec.
		}
	}

}

FGameplayTag UJxxExtensionsBPLibrary::Conv_NameToGameplayTag(FName TagName)
{
	FGameplayTag Result = FGameplayTag::RequestGameplayTag(TagName);

	return Result;
}

APlayerController* UJxxExtensionsBPLibrary::GetPlayerControllerFromOwnerChain(AActor* OwnerActor)
{
	APlayerController* Result = nullptr;

	if (OwnerActor)
	{
		for (AActor* TestOwner = OwnerActor; IsValid(TestOwner); TestOwner = TestOwner->GetOwner())
		{
			if (TestOwner->IsA<APlayerController>())
			{
				Result = CastChecked<APlayerController>(TestOwner);
				break;
			}
		}
	}

	return Result;
}

bool UJxxExtensionsBPLibrary::EncompassesPoint(UShapeComponent* InShapeComp,FVector Point,float SphereRadius,float& OutDistanceToPoint)
{
	if (!IsValid(InShapeComp))
	{
		return false;
	}

	FVector ClosestPoint;
	float DistanceSqr;

	if (InShapeComp->GetSquaredDistanceToCollision(Point, DistanceSqr, ClosestPoint) == false)
	{
		OutDistanceToPoint = -1.f;

		return false;
	}

	OutDistanceToPoint = FMath::Sqrt(DistanceSqr);

	return DistanceSqr >= 0.f && DistanceSqr <= FMath::Square(SphereRadius);
}

bool UJxxExtensionsBPLibrary::GetMouseDeprojectInLevelViewport(FVector& WorldPos,FVector& WorldDir)
{
	WorldPos = FVector::ZeroVector;
	WorldDir = FVector::ZeroVector;

#if WITH_EDITOR

	if (GEditor && GEditor->GetActiveViewport())
	{
		FEditorViewportClient* eclient = (FEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());
		if (eclient)
		{
			FIntPoint mousepos;

			mousepos.X = eclient->GetCachedMouseX();

			mousepos.Y = eclient->GetCachedMouseY();

			FSceneViewFamily viewFamily = FSceneViewFamily::ConstructionValues(eclient->Viewport, eclient->GetScene(), eclient->EngineShowFlags);

			FSceneView* culScene = eclient->CalcSceneView(&viewFamily);

			culScene->DeprojectFVector2D(mousepos, WorldPos, WorldDir);
			return true;
		}
	}
#endif

	return false;
}

bool UJxxExtensionsBPLibrary::TestTagRequirements(const FGameplayTagContainer& Container,const TArray<FGameplayTag>& RequireTags,
	const TArray<FGameplayTag>& IgnoreTags,const FGameplayTagQuery& TagQuery)
{
	auto HasAllRequireTags = [&RequireTags, &Container]()
		{
			for (const FGameplayTag& OtherTag : RequireTags)
			{
				if (!Container.HasTagExact(OtherTag))
				{
					return false;
				}
			}
			return true;
		};

	auto HasAnyIgnoreTags = [&IgnoreTags, &Container]()
		{
			for (const FGameplayTag& OtherTag : IgnoreTags)
			{
				if (Container.HasTagExact(OtherTag))
				{
					return true;
				}
			}
			return false;
		};

	const bool bHasRequired = HasAllRequireTags();
	const bool bHasIgnored = HasAnyIgnoreTags();
	const bool bMatchQuery = TagQuery.IsEmpty() || TagQuery.Matches(Container);

	return  bHasRequired && !bHasIgnored && bMatchQuery;
}

void UJxxExtensionsBPLibrary::SafeEndTask(UGameplayTask* Target)
{
	if (IsValid(Target))
	{
		Target->EndTask();
	}
}

void UJxxExtensionsBPLibrary::SafeEndAsyncAction(UCancellableAsyncAction* Target)
{
	if (IsValid(Target))
	{
		Target->Cancel();
	}
}
