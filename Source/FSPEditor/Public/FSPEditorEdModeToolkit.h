// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FSPEditorEdMode.h"
#include "Toolkits/BaseToolkit.h"
#include "Widgets/Input/SNumericDropDown.h"

class FFSPEditorEdModeToolkit : public FModeToolkit
{
public:

	FFSPEditorEdModeToolkit();

	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SWidget> ToolkitWidget;

	// initialization panel
	FReply OnFSPInitialize();
	bool IsInitialized() const;
	FString MessageInitialization;
	FText GetInitializationMessage() const;
	FText GetIsInitializedSummary() const;
		
	// Camera edits
	FReply OnCameraTrackAdd();
	FReply OnCameraTrackSelect();
	FText GetCameraTracksSummary() const;

	// Object controls
	FReply OnObjectsModify() const;
	FReply OnObjectsReset() const;
	FText GetTransformationSummary() const;

	// Transformations
	int TransformationGroup;
	FText TransformationGroupText() const;
	void TransformationGroupTextCommitted(const FText& Text, ETextCommit::Type Arg);
	
	//TArray<SNumericDropDown<int>::FNamedValue> GetCameraTracksOptions() const;
	FFSPEditorEdMode* GetEdMode() const;
};
