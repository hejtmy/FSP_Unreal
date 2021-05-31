// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSPLogWriter.h"
#include "FSPLog.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FSP_API UFSPLog : public UActorComponent
{
	GENERATED_BODY()

public:
	UFSPLog();
	const FString RelativePath = "recordings";
	
	UFUNCTION(BlueprintCallable, Category = "FSPLogging", BlueprintPure = False)
	bool WriteLine(FString Text, bool AllowOverwrite) const;

	UFUNCTION(BlueprintCallable, Category = "FSPLogging", BlueprintPure = False)
	bool WriteMessage(FString Text) const;

	UFUNCTION(BlueprintCallable, Category = "FSPLogging", BlueprintPure = False)
	bool CreateFile(FString Text);

	UFUNCTION(BlueprintPure, Category = "FSPLogging")
	bool IsLogOpen() const;

protected:
	FSPLogWriter* Log;

private:
	int64 TodayUnixTimestamp;
	float GetTimestamp() const;
};
