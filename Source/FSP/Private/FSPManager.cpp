// Fill out your copyright notice in the Description page of Project Settings.


#include "FSPManager.h"

#include "GeneralProjectSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFSPManager::AFSPManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	
    SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	
    RootComponent = SceneRoot;
}

// Called when the game starts or when spawned
void AFSPManager::BeginPlay()
{
	Super::BeginPlay();
	
	/** Make sure asset was loaded and class was set */
	if(PauseMenuWidgetClass)
	{
		PauseMenuWidget = CreateWidget<UFSPInGamePauseMenu>(GetWorld(), PauseMenuWidgetClass);
		PauseMenuWidget->Manager = this;
	}
	/** Create the widget of our UUserWidget type (UGUserWidget) from the class we loaded from the Content Browser */

	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent -> bBlockInput = false;
		InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AFSPManager::TogglePauseMenu);
		EnableInput(GetWorld()->GetFirstPlayerController());
		
		UE_LOG(FSP, Warning, TEXT("Input setup"));
	}
	else
	{
		UE_LOG(FSP, Warning, TEXT("No input is being collected"));
	}
}


// Called every frame
void AFSPManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFSPManager::Pause_Implementation()
{
	PauseMenuWidget->AddToViewport();
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	bMenuOn = true;
}

void AFSPManager::Resume_Implementation()
{
	PauseMenuWidget->RemoveFromParent();
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	bMenuOn = false;
}

void AFSPManager::TogglePauseMenu()
{
	UE_LOG(FSP, Warning, TEXT("Menu toggled"));
	if(bMenuOn)
	{
		Resume();
	}
	else
	{
		Pause();
	}
}

bool AFSPManager::ValidateLogger()
{
	if(IsValid(Logger)) return true;
	UE_LOG(FSP, Warning, TEXT("Logger actor is not valid"));
	return false;
}

bool AFSPManager::ValidateRecorder()
{
	if(IsValid(Recorder)) return true;
	UE_LOG(FSP, Warning, TEXT("Recorder actor is not valid"));
	return false;
}

void AFSPManager::TakeScreenshots_Implementation()
{
	if(!(ValidateLogger() && ValidateRecorder()))
	{
		UE_LOG(FSP, Warning, TEXT("Objects are not valid, cannot make a recording"));
		return;
	}
	Recorder->CreateScreenshots(Logger);	
}

void AFSPManager::StopScreenshotting_Implementation()
{
	if(ValidateRecorder()) { return; }
	Recorder->StopScreenshotting();
}

void AFSPManager::MakeARecording_Implementation()
{
	if(!(ValidateLogger() && ValidateRecorder()))
	{
		UE_LOG(FSP, Warning, TEXT("Objects are not valid, cannot make a recording"));
		return;
	}
	Recorder->StartRecording(Logger);
}

void AFSPManager::FinishRecording_Implementation()
{
	if(ValidateRecorder()) return;
	Recorder->StopRecording();
}

