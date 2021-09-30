// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FSPEditorEdMode.h"
#include "Toolkits/BaseToolkit.h"

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
	bool IsInitialized();
	FReply OnCameraTrackAdd();
	
	FString MessageInitialization;
	FText GetInitializationMessage() const;
	FText GetTransformationSummary() const;
	FText GetTracksSummary() const;
	FFSPEditorEdMode* GetEdMode() const;
};
