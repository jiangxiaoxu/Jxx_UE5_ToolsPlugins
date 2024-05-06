// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedStructWrapperObject.h"

UInstancedStructWrapperObject::UInstancedStructWrapperObject()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		SetFlags(RF_StrongRefOnFrame);
	}
}
