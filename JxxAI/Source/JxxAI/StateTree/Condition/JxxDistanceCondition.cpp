// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxDistanceCondition.h"

#include "StateTreeExecutionContext.h"

namespace 
{
	template <typename T>
	bool CompareNumbers(const T Left,const T Right,const EGenericAICheck Operator)
	{
		switch (Operator)
		{
		case EGenericAICheck::Equal:
			return Left == Right;
			break;
		case EGenericAICheck::NotEqual:
			return Left != Right;
			break;
		case EGenericAICheck::Less:
			return Left < Right;
			break;
		case EGenericAICheck::LessOrEqual:
			return Left <= Right;
			break;
		case EGenericAICheck::Greater:
			return Left > Right;
			break;
		case EGenericAICheck::GreaterOrEqual:
			return Left >= Right;
			break;
		default:
			ensureMsgf(false, TEXT("Unhandled operator %d"), Operator);
			return false;
			break;
		}
		return false;
	}
}

bool FJxxDistanceCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);


	if (InstanceData.bSourceUseActor && !IsValid(InstanceData.SourceActor))
	{
		return false;
	}

	if (!InstanceData.bSourceUseActor && !InstanceData.SourceLocationEntry.IsValid())
	{
		return false;
	}

	if (InstanceData.bTargetUseActor && !IsValid(InstanceData.TargetActor))
	{
		return false;
	}

	if (!InstanceData.bTargetUseActor && !InstanceData.TargetLocationEntry.IsValid())
	{
		return false;
	}


	const FVector Source = InstanceData.bSourceUseActor ?
		InstanceData.SourceActor->GetActorLocation() : InstanceData.SourceLocationEntry.GetTransform().GetValue().GetLocation();

	const FVector Target = InstanceData.bTargetUseActor ?
		InstanceData.TargetActor->GetActorLocation() : InstanceData.TargetLocationEntry.GetTransform().GetValue().GetLocation();

	const FVector::FReal Left = InstanceData.bIgnoreZ ? FVector::Dist2D(Source, Target) : FVector::Dist(Source, Target);
	const FVector::FReal Right = FMath::Abs(InstanceData.Distance);
	const double Tolerance = FMath::Abs(InstanceData.Tolerance);

	bool bResult = false;

	if (InstanceData.Operator==EJxxDistanceCheck::Less)
	{
		bResult=(Left < Right);
	}
	else if(InstanceData.Operator == EJxxDistanceCheck::NearlyEqual)
	{
		bResult = FMath::IsNearlyEqual(Left, Right, Tolerance);
	}
	else if (InstanceData.Operator == EJxxDistanceCheck::Greater)
	{
		bResult = (Left > Right);
	}
	else if (InstanceData.Operator == EJxxDistanceCheck::InRange)
	{
		const bool GreaterX = Left > FMath::Abs(InstanceData.DistanceRange.X);
		const bool BelowY = Left < FMath::Abs(InstanceData.DistanceRange.Y);

		bResult = (GreaterX && BelowY);
	}
	else
	{
		checkNoEntry();
		return bResult;
	}

	return bResult ^ bInvert;
}
