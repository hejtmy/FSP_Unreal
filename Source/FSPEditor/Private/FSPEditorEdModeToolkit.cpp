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
				.FillHeight(0.2)
				.HAlign(HAlign_Center)
				.Padding(5)
				[
					SNew(SVerticalBox) +
						SVerticalBox::Slot()
						.FillHeight(0.1)
						.HAlign(HAlign_Center)
						[
							SNew(SButton)
							.OnClicked(this, &FFSPEditorEdModeToolkit::OnFSPInitialize)
							.HAlign(HAlign_Center)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("FSPEditorEdModeInitializeBtn", "Initialize FSP"))
							]
						] +
						SVerticalBox::Slot()
						.AutoHeight()
						.HAlign(HAlign_Center)
						.Padding(5)
						[
							SNew(STextBlock)
							.Text(this, &FFSPEditorEdModeToolkit::GetIsInitializedSummary)
							//.Text_Lambda([this]()->FFSPEditorEdModeToolkit::GetInitializationMessage)
							//.Text(LOCTEXT("FSPEditorEdModeInitializeMessage", "Not initialized"))
						]
				] +
				// -------- CAMERA TRACK CONTROLS -----------------
				SVerticalBox::Slot()
				.FillHeight(0.4)
				.HAlign(HAlign_Center)
				[
					SNew(SVerticalBox)
					.IsEnabled(&FFSPEditorEdModeToolkit::IsInitialized) +
					SVerticalBox::Slot()
					.FillHeight(0.2)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("FSPEditorEdModeCameraTrackTitleText", "Camera controls track"))
					] +
					SVerticalBox::Slot()
					.FillHeight(0.1)
					[
						SNew(SButton)
						.Text(LOCTEXT("FSPEditorEdModeAddCameraTrackBtn", "Add Camera Track"))
						.OnClicked(this, &FFSPEditorEdModeToolkit::OnCameraTrackAdd)
					] +
					SVerticalBox::Slot()
					.FillHeight(0.1)
					[
						SNew(STextBlock)
						.Text(this, &FFSPEditorEdModeToolkit::GetCameraTracksSummary)
					] +
					SVerticalBox::Slot()
					.FillHeight(0.2)
					[
						SNullWidget::NullWidget
						//SNew(SNumericDropDown<int>)
						//.DropDownValues(FFSPEditorEdModeToolkit::GetCameraTracksOptions())
					] +
					SVerticalBox::Slot()
					[
						SNullWidget::NullWidget
					]
				] +
				// ------------ FSP object transformations ----------------------
				SVerticalBox::Slot()
				.FillHeight(0.3)
				.HAlign(HAlign_Center)
				[
					SNew(SVerticalBox)
					.IsEnabled(&FFSPEditorEdModeToolkit::IsInitialized) +
					SVerticalBox::Slot()
					.FillHeight(0.2)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("FSPEditorEdModeObjectControlsTitleText","Object controls"))
					] +
					SVerticalBox::Slot()
					.FillHeight(0.2)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("FSPEditorEdModeObjectControlsTransformationTitleText","Transformation control"))
					] +
					SVerticalBox::Slot()
					[
						SNew(SVerticalBox) +
						SVerticalBox::Slot()[
							SNew(SHorizontalBox) +
							SHorizontalBox::Slot()[
								SNew(SEditableTextBox)
								.Text(this, &FFSPEditorEdModeToolkit::TransformationGroupText)
							]
						] +
						SVerticalBox::Slot()[
							SNew(SHorizontalBox) +
							SHorizontalBox::Slot()
							[
								SNew(SButton)
								.Text(LOCTEXT("FSPEditorEdModeObjectControlsResetAllBtn","Reset all objects"))
								.OnClicked(this, &FFSPEditorEdModeToolkit::OnObjectsReset)
							] +
							SHorizontalBox::Slot()
							[
								SNew(SButton)
								.Text(LOCTEXT("FSPEditorEdModeObjectControlsModifyAllBtn","transform all objects"))
								.OnClicked(this, &FFSPEditorEdModeToolkit::OnObjectsModify)
							]
						]
					] +
					SVerticalBox::Slot()
					.AutoHeight()
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

bool FFSPEditorEdModeToolkit::IsInitialized() const
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

FText FFSPEditorEdModeToolkit::GetIsInitializedSummary() const
{
	return FText::FromString(IsInitialized() ? TEXT("The FSP is initialized and ready to use") :
		TEXT("FSP is not initalized"));
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

FText FFSPEditorEdModeToolkit::GetCameraTracksSummary() const
{
	const auto Tracks = GetEdMode()->GetCameraTracks();
	const int32 nTracks = Tracks.Num();
	FFormatNamedArguments Args;
	Args.Add(TEXT("NumberOfTracks"), FText::AsNumber(nTracks));
	return FText::Format(LOCTEXT("FSPEditorEdModeCameraTrackSummaryTxt", "{NumberOfTracks}"), Args);
}

FReply FFSPEditorEdModeToolkit::OnObjectsModify() const
{
	return GetEdMode()->ApplyObjectsTransformations() ? FReply::Handled() : FReply::Unhandled();
}

FReply FFSPEditorEdModeToolkit::OnObjectsReset() const
{
	return GetEdMode()->ResetObjectsTransformations() ? FReply::Handled() : FReply::Unhandled();
}

FReply FFSPEditorEdModeToolkit::OnCameraTrackSelect()
{
	 return FReply::Unhandled(); 
}

FText FFSPEditorEdModeToolkit::TransformationGroupText() const
{
	return TransformationGroup;
}
/*
TArray<SNumericDropDown<int>::FNamedValue> FFSPEditorEdModeToolkit::GetCameraTracksOptions() const
{
	TArray<SNumericDropDown<int>::FNamedValue> CameraTracks;
	int i = 0;
	for(auto Track : GetEdMode()->GetCameraTracks())
	{
		CameraTracks.Add(SNumericDropDown<int>::FNamedValue(i, LOCTEXT("track", "first"),
		LOCTEXT("SnapDescription_OneThousandth", "Set snap to 1/1000th")));
		i++;	
	}
	return CameraTracks;
}
*/

FFSPEditorEdMode* FFSPEditorEdModeToolkit::GetEdMode() const
{
	return static_cast<FFSPEditorEdMode*>(GetEditorMode());
}

class FEdMode* FFSPEditorEdModeToolkit::GetEditorMode() const
{
	return (GLevelEditorModeTools().GetActiveMode(FFSPEditorEdMode::EM_FSPEditorEdModeId));
}

#undef LOCTEXT_NAMESPACE
