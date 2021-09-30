// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSPEditorEdMode.h"

#include "EditorLevelUtils.h"
#include "FSPEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "FSPEditorUnrealLogDeclarations.h"
#include "FSPGameMode.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"

const FEditorModeID FFSPEditorEdMode::EM_FSPEditorEdModeId = TEXT("EM_FSPEditorEdModeId");

FFSPEditorEdMode::FFSPEditorEdMode()
{
}

FFSPEditorEdMode::~FFSPEditorEdMode()
{

}

void FFSPEditorEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FFSPEditorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FFSPEditorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FFSPEditorEdMode::UsesToolkits() const
{
	return true;
}

FString FFSPEditorEdMode::FSPInitialize()
{
	struct HelperFuncs
	{
		static TArray<AActor*> FindActors(TSubclassOf<AActor> Class, UWorld* World)
		{
			static TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(World, Class, FoundActors);
			return FoundActors;
		}
		static void ClearActors(TArray<AActor*> Actors)
		{
			for(int i = Actors.Num()-1; i >= 0; i--)
			{
				AActor* Actor = Actors[i];
				Actors.RemoveAt(i);
				Actor->Destroy();
			}
		}
		static AActor* CreateSingleInstance(const TSubclassOf<AActor> SpawnClass, const TSubclassOf<AActor> ParentClass,
			UWorld* World)
		{
			const TArray<AActor*> ParentObjects = FindActors(ParentClass, World);
			ClearActors(ParentObjects);
			const TArray<AActor*> Objects = FindActors(SpawnClass, World);
			ClearActors(Objects);
			AActor* Object = World->SpawnActor(SpawnClass);
			return Object;
		}
	};
	
	FName FolderPath = "/FSP/ControlObjects";
	// FSP Experiment manager

	// FSPObjectsManager
	//TArray<AActor*> ObjectManagers = HelperFuncs::FindActors(AFSPObjectManager::StaticClass(), GetWorld());
	//HelperFuncs::ClearActors(ObjectManagers);
	//AActor* ObjectManager = GetWorld()->SpawnActor<AFSPObjectManager>();
	UWorld* World =	GetWorld();
	//AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);
	//AGameModeBase* GameMode = World->GetAuthGameMode();
	const TSubclassOf<AGameModeBase> GameMode = World->GetWorldSettings()->DefaultGameMode;
	UE_LOG(FSPEditor, Display, TEXT("Game mode found %d - %d"), (GameMode != nullptr),
		GameMode->IsChildOf(AFSPGameMode::StaticClass()));
	
	if(!GameMode->IsChildOf(AFSPGameMode::StaticClass()))
	{
		return TEXT("Game mode not set to a valid FSP game mode");
	}

	TSubclassOf<AFSPGameMode> FSPGameModeType = *GameMode;
	
	AFSPGameMode* FSPGameMode = NewObject<AFSPGameMode>(World, FSPGameModeType);
	UE_LOG(FSPEditor, Display, TEXT("%s"), *(FSPGameMode->Manager->GetClass()->GetName()));
	UE_LOG(FSPEditor, Display, TEXT("%d"), (FSPGameMode->Manager->IsChildOf(AFSPManager::StaticClass())));

	ObjectManager = static_cast<AFSPObjectManager*>(HelperFuncs::CreateSingleInstance(
		FSPGameMode->ObjectManager, AFSPObjectManager::StaticClass(), World));
	ObjectManager->SetFolderPath(FolderPath);

	Pawn = static_cast<AFSPPawn*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Pawn,
		AFSPPawn::StaticClass(), World));
	
	Recorder = static_cast<AFSPRecorder*>(HelperFuncs::CreateSingleInstance(
		FSPGameMode->Recorder, AFSPRecorder::StaticClass(), World));
	Recorder->SetFolderPath(FolderPath);
	Recorder->ObjectManager = ObjectManager.Get();
	Recorder->Pawn = Pawn;

	// FSP logger
	Logger = static_cast<AFSPLogger*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Logger,
		AFSPLogger::StaticClass(), World));
	Logger->SetFolderPath(FolderPath);
	
	Manager = static_cast<AFSPManager*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Manager,
		AFSPManager::StaticClass(), World));
	Manager->SetFolderPath(FolderPath);
	Manager->Logger = Logger;
	Manager->ObjectManager = ObjectManager.Get();
	Manager->Recorder = Recorder;
	Manager->Pawn = Pawn;
	
	AddKeyMappings();
	
	return TEXT("FSP initialized");
}

bool FFSPEditorEdMode::AddCameraTrack()
{
	if(!IsInitialized())
	{
		UE_LOG(FSPEditor, Display, TEXT("Initialize the FSP first"));
		return false;
	}
	AFSPTrack* Track = GetWorld()->SpawnActor<AFSPTrack>();
	if(!IsValid(Track)) return false;
	CameraTracks.Add(Track);
	Pawn->TrackRider->Track = Track;
	return true;
}

bool FFSPEditorEdMode::IsInitialized() const
{
	if(!IsValid(Recorder)) return false;
	if(!ObjectManager.IsValid()) return false;
	if(!IsValid(Logger)) return false;
	if(!IsValid(Pawn)) return false;
	return true;
}

void FFSPEditorEdMode::AddKeyMappings() const
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();
	if(Settings == nullptr)
	{
		UE_LOG(FSPEditor, Warning, TEXT("Failed to get input settings"));
		return;
	}
	Settings->AddActionMapping(FInputActionKeyMapping(FName("Pause"), EKeys::Escape));
	Settings->AddActionMapping(FInputActionKeyMapping(FName("Resume"), EKeys::Escape));
	Settings->AddActionMapping(FInputActionKeyMapping(FName("Pause"), EKeys::E));
	Settings->AddActionMapping(FInputActionKeyMapping(FName("Resume"), EKeys::E));
	Settings->SaveKeyMappings();
	UE_LOG(FSPEditor, Display, TEXT("Actions mapped"));
}

bool FFSPEditorEdMode::TryReinitialize()
{
	return true;
}
