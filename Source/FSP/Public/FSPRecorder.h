// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSPLogger.h"
#include "GameFramework/Actor.h"
#include "FSPPawn.h"
#include "FSPSceneAnalyzer.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP|Analysis")
	UFSPSceneAnalyzer* SceneAnalyzer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP|Analysis")
	AFSPObjectManager* ObjectManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Analysis")
	int32 Precision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Recording")
	int32 SceneAnalysisLoggingFrequency = 50;

	/**
	 *@brief Starts recording and logging positional and scene information with passed logger
	 **/
	UFUNCTION(BlueprintCallable, Category="FSP|Recording")
	void StartRecording(AFSPLogger* Logging);

	/** Starts recording without any logging **/
	UFUNCTION(BlueprintCallable, Category="FSP|Recording")
	void StartRecordingWithoutLogging();
	
	UFUNCTION()
	void LogSceneAnalysis();


	UFUNCTION(BlueprintCallable, Category="FSP")
	void StopRecording();

	/**
	 * @brief Creates given number of screenshots and logs the scene and position information
	 */
	UFUNCTION(BlueprintCallable, Category="FSP")
	void CreateScreenshots(AFSPLogger* Logging);

	/**
	 * @brief Creates a single screenshot from the position in which player is standing. No information is logged
	 */
	UFUNCTION(BlueprintCallable, Category="FSP|Screenshots")
	void CreateScreenshot();

	UFUNCTION(BlueprintCallable, Category="FSP|Screenshots")
	void StopScreenshotting();

	UPROPERTY(BlueprintAssignable, Category="FSP|Screenshots")
	FScreenshotsStateChanged OnScreenshotTaken;

	UPROPERTY(BlueprintAssignable, Category="FSP|Screenshots")
	FScreenshotsStateChanged OnScreenshotsFinished;
	
	UPROPERTY(BlueprintAssignable, Category="FSP|Screenshots")
	FScreenshotsStateChanged OnScreenshotsStopped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Screenshots")
	int32 ScreenshotWidth = 3840;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FSP|Screenshots")
	int32 ScreenshotHeight = 2160;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "FSP|Required")
	AFSPPawn* Pawn;

	UPROPERTY(EditAnywhere, Category="FSP|Screenshots")
	int nScreenshots = 30;

	UPROPERTY(EditAnywhere, Category="FSP|Screenshots")
	float ScreenshotDelay = 3.0f;

	bool isRecording = false;

	UFUNCTION()
	void FinishRecording();

	void ResetSceneRecordingIndex();
	
private:
	UPROPERTY()
	AFSPLogger* Logger;
	// Screenshotting
	bool bIsScreenshotting;
	FTimerHandle ScreenshottingHandle;
	float LastScreenshotTrack;
	void CreateNextScreenshot();
	void FinishScreenshotting();
	void LogScreenshotPlayerPosition();

	// Logging
	FTimerHandle SceneAnalysisLoggingHandle;
	int32 iSceneRecording; //index of individual scene logs
};