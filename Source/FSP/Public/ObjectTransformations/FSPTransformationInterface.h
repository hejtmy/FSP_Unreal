// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FSPTransformationInterface.generated.h"

/**
 * 
 */

UINTERFACE(BlueprintType)
class FSP_API UFSPTransformationInterface : public UInterface
{
	GENERATED_BODY()
};

class FSP_API IFSPTransformationInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FSP|Transformation")
	void ChangeState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="FSP|Transformation")
	void ResetState();
};
