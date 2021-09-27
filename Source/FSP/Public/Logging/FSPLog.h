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
	
	UFUNCTION(BlueprintCallable, Category = "FSP|Logging", BlueprintPure = False)
	bool CreateFile(FString Text);
	
	UFUNCTION(BlueprintCallable, Category = "FSP|Logging", BlueprintPure = False)
	bool WriteLine(FString Text, bool AllowOverwrite = true) const;

	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = False)
	bool WriteArray(TArray<FString> Text, bool AllowOverwrite = true, FString Delim = ";", bool AddTimestamp = true) const;

	UFUNCTION(BlueprintCallable, Category = "FSP|Logging", BlueprintPure = False)
	bool WriteMessage(FString Text) const;

	UFUNCTION(BlueprintCallable, Category="FSP|Logging")
	void CloseFile();
	
	UFUNCTION(BlueprintGetter, Category = "FSP|Logging")
	bool IsLogOpen() const;

protected:
	FSPLogWriter* Log;

private:
	int64 TodayUnixTimestamp;
	float GetTimestamp() const;
	FString CreateTimestamp() const;
};
