// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSPManager.h"
#include "FSPRecorder.h"
#include "GameFramework/GameModeBase.h"
#include "FSPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FSP_API AFSPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFSPGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP")
	TSubclassOf<AFSPManager> Manager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP")
	TSubclassOf<AFSPObjectManager> ObjectManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP")
	TSubclassOf<AFSPRecorder> Recorder;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP")
	TSubclassOf<AFSPLogger> Logger;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FSP")
	TSubclassOf<AFSPPawn> Pawn;
};
