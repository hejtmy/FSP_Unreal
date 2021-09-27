// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSPEditorEdMode.h"

#include "EditorLevelUtils.h"
#include "FSPEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "FSPEditorUnrealLogDeclarations.h"
#include "FSPGameMode.h"
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
			const TArray<AActor*> Objects = FindActors(ParentClass, World);
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
		FSPGameMode->Manager, AFSPObjectManager::StaticClass(), World));
	ObjectManager->SetFolderPath(FolderPath);

	Recorder = static_cast<AFSPRecorder*>(HelperFuncs::CreateSingleInstance(
		FSPGameMode->Recorder, AFSPRecorder::StaticClass(), World));
	Recorder->SetFolderPath(FolderPath);
	Recorder->ObjectManager = ObjectManager;

	// FSP logger
	Logger = static_cast<AFSPLogger*>(HelperFuncs::CreateSingleInstance(FSPGameMode->Logger,
		AFSPLogger::StaticClass(), World));
	Logger->SetFolderPath(FolderPath);

	return TEXT("FSP initialized");
}

bool FFSPEditorEdMode::IsInitialized() const
{
	if(Recorder == nullptr) return false;
	if(ObjectManager == nullptr) return false;
	if(Logger == nullptr) return false;
	return true;
}
