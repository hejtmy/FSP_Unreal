// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPInGamePauseMenu.h"
#include "FSPObjectManager.h"
#include "FSPRecorder.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSP")
	AFSPObjectManager* ObjectManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSP")
	AFSPRecorder* Recorder;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSP")
	AFSPLogger* Logger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSP")
	AFSPPawn* Pawn;

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void MakeARecording();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void FinishRecording();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void TakeScreenshots();

	UFUNCTION(BlueprintNativeEvent, Category="FSP")
	void StopScreenshotting();
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSP|UI")
	UFSPInGamePauseMenu* PauseMenuWidget;


private:
	bool ValidataLogger();	
	bool ValidataRecorder();	
};
