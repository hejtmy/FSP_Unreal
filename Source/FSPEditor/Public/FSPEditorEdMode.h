// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "FSPGameMode.h"
#include "FSPManager.h"
#include "FSPObjectManager.h"
#include "FSPRecorder.h"

class FFSPEditorEdMode: public FEdMode
{
public:
	const static FEditorModeID EM_FSPEditorEdModeId;
public:
	FFSPEditorEdMode();
	virtual ~FFSPEditorEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

	// Initialization panel
	
	/** Initialize and binds manager objects and adds proper keymappings to the open scene
	 * @return Returns message about the result to be showed in the slate
	 **/
	FString FSPInitialize();

	/**
	 * Adds a camera track to the scene
	 * @return returns if this addition was successful
	 */
	bool AddCameraTrack();
	
	bool IsInitialized() const;

	TWeakObjectPtr<AFSPObjectManager> GetObjectManager() const {return ObjectManager;}
	TArray<AFSPTrack*> GetCameraTracks() const {return CameraTracks; }

	// Item transformation panel
	bool ApplyObjectsTransformations();
	bool ResetObjectsTransformations();
	bool GetFSPObjects(TArray<UFSPObject*> &Out) const;

protected:
	TWeakObjectPtr<AFSPObjectManager> ObjectManager;
	TWeakObjectPtr<AFSPRecorder> Recorder;
	TWeakObjectPtr<AFSPLogger> Logger;
	TWeakObjectPtr<AFSPManager> Manager;
	TWeakObjectPtr<AFSPPawn> Pawn;
	TArray<AFSPTrack*> CameraTracks;
	
	void AddKeyMappings() const;
	// Camera track panel

	/**
	 * Tries to collect existing managers and loggers into the fields. 
	 */
	bool TryReinitialize();
	AFSPGameMode* GetGameMode(UWorld* World) const;


	void CollectCameraTracks();
	void CollectFSPObjects() const;
private:
	
	static TArray<AActor*> FindActors(const TSubclassOf<AActor> Class, const UWorld* World);
	void AssignRelations() const;

};


