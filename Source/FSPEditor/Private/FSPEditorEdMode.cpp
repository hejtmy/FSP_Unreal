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
	
	TryReinitialize();
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
	UWorld* World =	GetWorld();

	AFSPGameMode* FSPGameMode = GetGameMode(World);
	if(FSPGameMode == nullptr)
	{
		return TEXT("Game mode not set to a valid FSP game mode");
	}
	
	ObjectManager = static_cast<AFSPObjectManager*>(HelperFuncs::CreateSingleInstance(
		FSPGameMode->ObjectManager, AFSPObjectManager::StaticClass(), World));
	ObjectManager->SetFolderPath(FolderPath);

	Pawn = static_cast<AFSPPawn*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Pawn,
		AFSPPawn::StaticClass(), World));
	
	Recorder = static_cast<AFSPRecorder*>(HelperFuncs::CreateSingleInstance(
		FSPGameMode->Recorder, AFSPRecorder::StaticClass(), World));
	Recorder->SetFolderPath(FolderPath);

	// FSP logger
	Logger = static_cast<AFSPLogger*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Logger,
		AFSPLogger::StaticClass(), World));
	Logger->SetFolderPath(FolderPath);
	
	Manager = static_cast<AFSPManager*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Manager,
		AFSPManager::StaticClass(), World));
	Manager->SetFolderPath(FolderPath);

	AssignRelations();
	AddKeyMappings();
	return TEXT("FSP initialized");
}

void FFSPEditorEdMode::AssignRelations() const
{
	Recorder->ObjectManager = ObjectManager.Get();
	Recorder->Pawn = Pawn.Get();
	Manager->Logger = Logger.Get();
	Manager->ObjectManager = ObjectManager.Get();
	Manager->Recorder = Recorder.Get();
	Manager->Pawn = Pawn.Get();
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
	if(!Recorder.IsValid()) return false;
	if(!ObjectManager.IsValid()) return false;
	if(!Logger.IsValid()) return false;
	if(!Pawn.IsValid()) return false;
	if(!Manager.IsValid()) return false;
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
	UE_LOG(FSPEditor, Display, TEXT("Trying to find existing objects"));
	struct Temp
	{
		static AActor* GetSingleInstance(const TSubclassOf<AActor> SpawnClass, UWorld* World)
		{
			const TArray<AActor*> Objects = FindActors(SpawnClass, World);
			if(Objects.Num() != 1 ) return nullptr;
			return Objects[0];
		}
	};
	
	UWorld* World = GetWorld();
	AFSPGameMode* GameMode = GetGameMode(World);
	if(GameMode == nullptr)
	{
		UE_LOG(FSPEditor, Warning, TEXT("Valid Game mode not found"));
		return false;
	}
	
	UE_LOG(FSPEditor, Warning, TEXT("Finding ObjectManager"));
	ObjectManager = static_cast<AFSPObjectManager*>(Temp::GetSingleInstance(GameMode->ObjectManager, World));
	if(ObjectManager == nullptr) return false;
	
	UE_LOG(FSPEditor, Warning, TEXT("Finding Pawn"));
	Pawn = static_cast<AFSPPawn*>(Temp::GetSingleInstance(GameMode->Pawn, World));
	if(Pawn == nullptr) return false;
	
	UE_LOG(FSPEditor, Warning, TEXT("Finding Recorder"));
	Recorder = static_cast<AFSPRecorder*>(Temp::GetSingleInstance(GameMode->Recorder, World));
	if(Recorder == nullptr) return false;

	UE_LOG(FSPEditor, Warning, TEXT("Finding Logger"));
	Logger = static_cast<AFSPLogger*>(Temp::GetSingleInstance(GameMode->Logger, World));
	if(Logger == nullptr) return false;

	UE_LOG(FSPEditor, Warning, TEXT("Finding Manager"));
	Manager = static_cast<AFSPManager*>(Temp::GetSingleInstance(GameMode->Manager, World));
	if(Manager == nullptr) return false;

	AssignRelations();
	return true;
}

AFSPGameMode* FFSPEditorEdMode::GetGameMode(UWorld* World) const
{
	const TSubclassOf<AGameModeBase> GameMode = World->GetWorldSettings()->DefaultGameMode;
	UE_LOG(FSPEditor, Display, TEXT("Game mode found %d - %d"), (GameMode != nullptr),
		GameMode->IsChildOf(AFSPGameMode::StaticClass()));
	
	if(!GameMode->IsChildOf(AFSPGameMode::StaticClass()))
	{
		return nullptr;
	}

	const TSubclassOf<AFSPGameMode> FSPGameModeType = *GameMode;
	AFSPGameMode* FSPGameMode = NewObject<AFSPGameMode>(World, FSPGameModeType);
	return FSPGameMode;
}

TArray<AActor*> FFSPEditorEdMode::FindActors(const TSubclassOf<AActor> Class, const UWorld* World)
{
	static TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, Class, FoundActors);
	return FoundActors;
}
