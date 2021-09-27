// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "FSPInGamePauseMenu.generated.h"

class AFSPManager;
/**
 * 
 */
UCLASS()
class FSP_API UFSPInGamePauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, Category="FSP")
	AFSPManager* Manager;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonResume;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonTakeScreenshots;
		
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonRecordVideo;
		
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonQuit;
	
	UFUNCTION(BlueprintNativeEvent, Category="FSP|UI")
	void Resume();
	virtual void Resume_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category="FSP|UI")
	void RecordVideo();
	virtual void RecordVideo_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category="FSP|UI")
	void TakeScreenshots();
	virtual void TakeScreenshots_Implementation();

	UFUNCTION()
	void Quit();
	
};
