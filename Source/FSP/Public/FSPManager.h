// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FSPInGamePauseMenu.h"
#include "FSPObjectManager.h"
#include "FSPRecorder.h"
#include "Logging/FSPLogger.h"
#include "GameFramework/Actor.h"
#include "FSPManager.generated.h"

UCLASS()
class FSP_API AFSPManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSPManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneRoot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP")
	AFSPObjectManager* ObjectManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP")
	AFSPRecorder* Recorder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP")
	AFSPLogger* Logger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSP")
	AFSPPawn* Pawn;

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void MakeARecording();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void FinishRecording();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void TakeScreenshots();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void StopScreenshotting();

	UFUNCTION(BlueprintCallable, Category="FSP")
	void TogglePauseMenu();
	
	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void Pause();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void Resume();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSP|UI")
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	UPROPERTY()
	UFSPInGamePauseMenu* PauseMenuWidget;


private:
	bool ValidateLogger();	
	bool ValidateRecorder();

	bool bMenuOn;
};
