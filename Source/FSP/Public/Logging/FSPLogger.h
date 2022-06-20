// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSPLog.h"
#include "FSPObject.h"

#include "FSPLogger.generated.h"

UCLASS()
class FSP_API AFSPLogger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSPLogger();

	UFUNCTION(BlueprintCallable)
	void WriteToPositionLog(TArray<FString> Messages) const;

	UFUNCTION(BlueprintCallable, Category="FSP|Logging|Helpers")
	static FString LocationToString(FVector Location);
	static FString ScreenPositionToString(FVector2D& Position);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldWrite; 

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="FSP", meta=(ClampMin="1", ClampMax="100"))
	int32 LoggingFrequency = 20;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UFSPLog* SceneAnalysisLog;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UFSPLog* PositionLog;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UFSPLog* ScreenPositionLog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsLoggingPosition;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootSceneComponent;
	
public:	
	UFUNCTION(BlueprintCallable, Category="FSP|Logging")
	bool StartLoggingPosition(AActor* Object);

	UFUNCTION(BlueprintCallable, Category="FSP|Logging")
	void StopLoggingPosition();

	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = false)
	void LogPosition(AActor* Object, int32 iAnalysis = -1) const;

	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = false)
	bool LogSceneAnalysis(TMap<FName, int32> Results, int32 iAnalysis = -1);

	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = false)
	void LogObjectsPositions(TArray<UFSPObject*> Objects) const;

	UFUNCTION(BlueprintCallable)
	void LogObjectScreenPosition(FVector2D& Position, FString& Name, int32 iAnalysis = -1);

private:
	UPROPERTY()
	AActor* ObjectBeingTracked;

	FTimerHandle PositionLoggingHandle;

	float LastPositionLoggedTime;

	void CreatePositionLog() const;
	void CreateSceneAnalysisLog() const;
	void CreateScreenPositionLog() const;

};
