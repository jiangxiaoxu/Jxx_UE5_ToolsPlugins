// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InstancedStructWrapperObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "InstancedStructWrapperBPLibrary.generated.h"

struct FGenericStruct;
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
UCLASS()
class UInstancedStructWrapperBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, CustomThunk, Category = DataRegistry, meta = (DisplayName = "Create Instanced Struct Wrapper Object", CompactNodeTitle = "Struct=>InstObj", CustomStructureParam = "Value"))
	static UInstancedStructWrapperObject* CreateInstancedStructWrapperObject(const FGenericStruct& Value);

	DECLARE_FUNCTION(execCreateInstancedStructWrapperObject)
	{		
		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

		P_FINISH;

		UInstancedStructWrapperObject* Obj = nullptr;

		if (StructProp && StructProp->Struct && StructAddr)
		{
			Obj = NewObject<UInstancedStructWrapperObject>(GetTransientPackage(),NAME_None);

			Obj->InstancedStruct.InitializeAs(StructProp->Struct, (const uint8*)StructAddr);
		}
		else
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("CreateInstancedStructWrapperObject But Value Pin not Connect to Struct")), ELogVerbosity::Error);
		}

		*(UInstancedStructWrapperObject**)RESULT_PARAM = Obj;
	}


	UFUNCTION(BlueprintCallable, CustomThunk, Category = DataRegistry, meta = (DisplayName = "Create Struct Wrapped Event Data", CompactNodeTitle = "Struct Wrapped Event Data", CustomStructureParam = "Value"))
	static FGameplayEventData CreateStructWrappedEventData(FGameplayEventData EventData,const FGenericStruct& Value);

	DECLARE_FUNCTION(execCreateStructWrappedEventData)
	{
		P_GET_STRUCT(FGameplayEventData, EventData)

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

		P_FINISH;

		FGameplayEventData Result = EventData;

		UInstancedStructWrapperObject* Obj = nullptr;

		if (StructProp && StructProp->Struct && StructAddr)
		{
			Obj = NewObject<UInstancedStructWrapperObject>(GetTransientPackage(), NAME_None);

			Obj->InstancedStruct.InitializeAs(StructProp->Struct, (const uint8*)StructAddr);

			Result.OptionalObject2= Obj;
		}
		else
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("CreateStructWrappedEventData But Value Pin not Connect to Struct")), ELogVerbosity::Error);
		}

		*(FGameplayEventData*)RESULT_PARAM = Result;
	}




	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Messaging", meta = (ExpandBoolAsExecs = "ReturnValue",DisplayName = "Get Instanced Struct Data", CompactNodeTitle = "InstObj=>Struct", CustomStructureParam = "OutData"))
		static bool GetInstancedStructData(UObject* Object, FGenericStruct& OutData);

	DECLARE_FUNCTION(execGetInstancedStructData)
	{
		P_GET_OBJECT(UObject, InObj);

		Stack.MostRecentPropertyAddress = nullptr;
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* OutDataPtr = Stack.MostRecentPropertyAddress;
		FStructProperty* OutDataStructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		P_FINISH;

		bool bSuccess = false;

		if (auto Obj=Cast<UInstancedStructWrapperObject>(InObj))
		{
			if (Obj->InstancedStruct.IsValid())
			{
				if (OutDataStructProp && OutDataStructProp->Struct && OutDataPtr)
				{
					if (OutDataStructProp->Struct == Obj->InstancedStruct.GetScriptStruct())
					{
						OutDataStructProp->Struct->CopyScriptStruct(OutDataPtr, Obj->InstancedStruct.GetMemory());
						bSuccess = true;
					}
					else
					{
						FFrame::KismetExecutionMessage(*FString::Printf(TEXT("GetInstancedStructData OutData Pin Struct Typs Mismatch InstancedStruct[%s] Connected [%s]  "),
							*GetNameSafe(Obj->InstancedStruct.GetScriptStruct()),
							*GetNameSafe(OutDataStructProp->Struct)),
							ELogVerbosity::Error);
					}
				}
				else
				{
					FFrame::KismetExecutionMessage(*FString::Printf(TEXT("GetInstancedStructData But OutData Pin not Connect to Struct")), ELogVerbosity::Error);
				}
			}
			else
			{
				FFrame::KismetExecutionMessage(*FString::Printf(TEXT("GetInstancedStructData But InstancedStruct In InObj is not Valid")), ELogVerbosity::Warning);
			}
		}
		else
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("GetInstancedStructData But InObj cast failed")), ELogVerbosity::Warning);
		}

		*(bool*)RESULT_PARAM = bSuccess;
	}


	UFUNCTION(BlueprintPure,meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FInstancedStruct GetInstancedStructFromGameplayEventData(const FGameplayEventData& EventData);

};
