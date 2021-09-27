// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FSPInGamePauseMenu.h"

#include "FSPManager.h"

void UFSPInGamePauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UFSPInGamePauseMenu::Quit);
	ButtonRecordVideo->OnClicked.AddUniqueDynamic(this, &UFSPInGamePauseMenu::RecordVideo);
	ButtonTakeScreenshots->OnClicked.AddUniqueDynamic(this, &UFSPInGamePauseMenu::TakeScreenshots);
	ButtonResume->OnClicked.AddUniqueDynamic(this, &UFSPInGamePauseMenu::Resume);
}


void UFSPInGamePauseMenu::Quit()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}


void UFSPInGamePauseMenu::RecordVideo_Implementation()
{
	Resume();
	Manager->MakeARecording();
}

void UFSPInGamePauseMenu::TakeScreenshots_Implementation()
{
	Resume();
	Manager->TakeScreenshots();
}

void UFSPInGamePauseMenu::Resume_Implementation()
{
	Manager->Resume();
}