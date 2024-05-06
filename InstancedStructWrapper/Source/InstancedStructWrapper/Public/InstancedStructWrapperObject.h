// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InstancedStruct.h"
#include "InstancedStructWrapperObject.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, BlueprintType)
class INSTANCEDSTRUCTWRAPPER_API UInstancedStructWrapperObject : public UObject
{
	GENERATED_BODY()


public:

	UInstancedStructWrapperObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InstancedStruct", meta = (ExposeOnSpawn))
	FInstancedStruct InstancedStruct;

	template<class StructType>
	static ThisClass* CreateInstancedStructWrapperObject(const StructType& InStruct)
	{
		UE::StructUtils::CheckStructType<StructType>();

		ThisClass* Obj = NewObject<ThisClass>(GetTransientPackage(), NAME_None);
		Obj->InstancedStruct = FInstancedStruct::Make(InStruct);

		return Obj;
	}
	
};
