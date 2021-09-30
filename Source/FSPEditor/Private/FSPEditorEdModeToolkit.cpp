// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSPEditorEdModeToolkit.h"

#include <string>

#include "FSPEditorEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "FSPEditorUnrealLogDeclarations.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "FFSPEditorEdModeToolkit"

FFSPEditorEdModeToolkit::FFSPEditorEdModeToolkit()
{
}

void FFSPEditorEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Fill)
		.Padding(5)
		.IsEnabled(true)
		[
			SNew(SVerticalBox) +
				// ---------- INITIALIZATION ----------------
				SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				.Padding(5)
				[
					SNew(SVerticalBox) +
						SVerticalBox::Slot()
						.HAlign(HAlign_Center)
						[
							SNew(SButton)
							.Text(LOCTEXT("FSPEditorEdModeInitializeBtn", "Initialize FSP"))
							.OnClicked(this, &FFSPEditorEdModeToolkit::OnFSPInitialize)
						] +
						SVerticalBox::Slot()
						.AutoHeight()
						.HAlign(HAlign_Center)
						.Padding(5)
						[
							SNew(STextBlock)
							.Text(this, &FFSPEditorEdModeToolkit::GetInitializationMessage)
							//.Text_Lambda([this]()->FFSPEditorEdModeToolkit::GetInitializationMessage)
							//.Text(LOCTEXT("FSPEditorEdModeInitializeMessage", "Not initialized"))
						]
				] +
				SVerticalBox::Slot()
				[
					SNew(SSeparator)
				] +
				// -------- CAMERA TRACK CONTROLS -----------------
				SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SVerticalBox)
					.IsEnabled(&FFSPEditorEdModeToolkit::IsInitialized) +
					SVerticalBox::Slot()
					[
						SNew(SButton)
						.Text(LOCTEXT("FSPEditorEdModeAddCameraTrackBtn", "Add Camera Track"))
						.OnClicked(this, &FFSPEditorEdModeToolkit::OnCameraTrackAdd)
					] +
					SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(this, &FFSPEditorEdModeToolkit::GetTracksSummary)
					]
				] +
				SVerticalBox::Slot()
				[
					SNew(SSeparator)
				] +
				SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SVerticalBox)
					.IsEnabled(&FFSPEditorEdModeToolkit::IsInitialized) +
					SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(this, &FFSPEditorEdModeToolkit::GetTransformationSummary)
					]
				] 
		];
	FModeToolkit::Init(InitToolkitHost);
	
	return;
	
	const float Factor = 256.0f;
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}
	};
	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]

		];
}


FName FFSPEditorEdModeToolkit::GetToolkitFName() const
{
	return FName("FSPEditorEdMode");
}

FText FFSPEditorEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("FSPEditorEdModeToolkit", "DisplayName", "FSPEditorEdMode Tool");
}

FReply FFSPEditorEdModeToolkit::OnFSPInitialize()
{
	MessageInitialization = GetEdMode()->FSPInitialize();
	return FReply::Handled();
}

bool FFSPEditorEdModeToolkit::IsInitialized()
{
	return GetEdMode()->IsInitialized();
}

FReply FFSPEditorEdModeToolkit::OnCameraTrackAdd()
{
	GetEdMode()->AddCameraTrack();
	// rebuild the Slate??
	return FReply::Handled();
}

FText FFSPEditorEdModeToolkit::GetInitializationMessage() const
{
	const FString Message = (MessageInitialization.IsEmpty()) ? TEXT("Not initialized") : MessageInitialization;
	return FText::FromString(Message);
}

FText FFSPEditorEdModeToolkit::GetTransformationSummary() const
{
	//const AFSPObjectManager* OM = GetEdMode()->GetObjectManager();
	TWeakObjectPtr<AFSPObjectManager> OM = GetEdMode()->GetObjectManager();
	if(!OM.IsValid()) return FText::FromString(TEXT("Manager not found"));
	//return FText::FromString(TEXT("Object manager is valid"));
	const auto Objects = OM->GetObjects();
	FString Message;
	if(Objects.Num() > 0)
	{
		Message = FString::Printf(TEXT("There are %d objects in the scene"), Objects.Num());
	}
	else
	{
		Message = TEXT("There are no FSP objects in the scene");
	}
	return FText::FromString(Message);
}

FText FFSPEditorEdModeToolkit::GetTracksSummary() const
{
	auto Tracks = GetEdMode()->GetCameraTracks();
	int32 nTracks = Tracks.Num();
	FFormatNamedArguments Args;
	Args.Add(TEXT("NumberOfTracks"), FText::AsNumber(nTracks));
	return FText::Format(LOCTEXT("FSPEditorEdModeCameraTrackSummaryTxt", "{NumberOfTracks}"), Args);
}

FFSPEditorEdMode* FFSPEditorEdModeToolkit::GetEdMode() const
{
	return static_cast<FFSPEditorEdMode*>(GetEditorMode());
}

class FEdMode* FFSPEditorEdModeToolkit::GetEditorMode() const
{
	return (GLevelEditorModeTools().GetActiveMode(FFSPEditorEdMode::EM_FSPEditorEdModeId));
}

#undef LOCTEXT_NAMESPACE
