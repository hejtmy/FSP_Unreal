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

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="FSP", meta=(ClampMin="20"))
	int32 LoggingFrequency = 50;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UFSPLog* SceneAnalysisLog;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UFSPLog* PositionLog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsLoggingPosition;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootSceneComponent;
	
private:
	UPROPERTY()
	AActor* ObjectBeingTracked;

	FTimerHandle PositionLoggingHandle;

	float LastPositionLoggedTime;

	FString LocationToString(FVector Location) const;
	
public:	
	UFUNCTION(BlueprintCallable, Category="FSP|Logging")
	bool StartLoggingPosition(AActor* Object);

	UFUNCTION(BlueprintCallable, Category="FSP|Logging")
	void StopLoggingPosition();

	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = false)
	void LogPosition(AActor* Object) const;
	
	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = false)
	bool LogSceneAnalysis(TMap<FName, int32> Results) const;

	UFUNCTION(BlueprintCallable, Category="FSP|Logging", BlueprintPure = false)
	void LogItemsPositions(TArray<UFSPObject*> Objects) const;
};
