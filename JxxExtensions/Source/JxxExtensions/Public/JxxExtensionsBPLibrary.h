// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "GameplayTask.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JxxExtensionsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS(meta = (DisplayName = "Extensions BP Library"))
class JXXEXTENSIONS_API UJxxExtensionsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "JxxExtensions sample test testing"), Category = "JxxExtensionsTesting")
	static float JxxExtensionsSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", Cooldown = "0.5", Keywords = "Exec"), Category = "Utilities|FlowControl")
	static void ExecuteWithCooldown(const UObject* WorldContextObject, float Cooldown, FLatentActionInfo LatentInfo);



	UFUNCTION(BlueprintPure, meta = (DisplayName = "To GameplayTag (Name)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities")
	static FGameplayTag Conv_NameToGameplayTag(FName TagName);

	UFUNCTION(BlueprintCallable)
	static APlayerController* GetPlayerControllerFromOwnerChain(AActor* OwnerActor);


	//进行测试Encompasses,判断是否点在ShapeComponent或者点到ShapeComponent的距离.
	UFUNCTION(BlueprintCallable, meta = (SphereRadius = 2.0f))
	static bool EncompassesPoint(UShapeComponent* InShapeComp, FVector Point, float SphereRadius, float& OutDistanceToPoint);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Level Utility", Meta = (ExpandBoolAsExecs = "ReturnValue", DevelopmentOnly))
	static bool GetMouseDeprojectInLevelViewport(FVector& WorldPos, FVector& WorldDir);

	UFUNCTION(BlueprintCallable, Category = "Tags")
	static bool TestTagRequirements(const FGameplayTagContainer& Container, const TArray<FGameplayTag>& RequireTags, const TArray<FGameplayTag>& IgnoreTags, const FGameplayTagQuery& TagQuery);


	UFUNCTION(BlueprintCallable)
	static void SafeEndTask(UGameplayTask* Target);

	UFUNCTION(BlueprintCallable)
	static void SafeEndAsyncAction(class UCancellableAsyncAction* Target);



};
