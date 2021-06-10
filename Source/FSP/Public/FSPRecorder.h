// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSPPawn.h"
#include "FSPRecorder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScreenshotsStateChanged);

UCLASS()
class FSP_API AFSPRecorder : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AFSPRecorder();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="FSP")
	void CreateScreenshots();

	UFUNCTION(BlueprintCallable, Category="FSP")
	void CreateScreenshot();

	UFUNCTION(BlueprintCallable, Category="FSP")
	void StopScreenshotting();

	UPROPERTY(BlueprintAssignable, Category="FSP")
	FScreenshotsStateChanged OnScreenshotTaken;

	UPROPERTY(BlueprintAssignable, Category="FSP")
	FScreenshotsStateChanged OnScreenshotsFinished;
	
	UPROPERTY(BlueprintAssignable, Category="FSP")
	FScreenshotsStateChanged OnScreenshotsStopped;

	UPROPERTY(EditAnywhere, Category="FSP Screenshots")
	int32 ScreenshotWidth = 3840;
	
	UPROPERTY(EditAnywhere, Category="FSP Screenshots")
	int32 ScreenshotHeight = 2160;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "FSP Required")
	AFSPPawn* Pawn;

	UPROPERTY(EditAnywhere, Category="FSP Screenshots")
	int nScreenshots = 30;

	UPROPERTY(EditAnywhere, Category="FSP Screenshots")
	float ScreenshotDelay = 3.0f;
	
	bool bIsScreenshotting;
	FTimerHandle ScreenshottingHandle;
	float LastScreenshotTrack;
	void CreateNextScreenshot();
	void FinishScreenshotting();
};