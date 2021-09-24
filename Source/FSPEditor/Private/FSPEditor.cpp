# include "FSPEditor.h"
# include "FSPEditorEdMode.h"
# include "FSPEditorUnrealLogDeclarations.h"

DEFINE_LOG_CATEGORY(FSPEditor)

#define LOCTEXT_NAMESPACE "FFSPEditorModule"

void FFSPEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FFSPEditorEdMode>(FFSPEditorEdMode::EM_FSPEditorEdModeId,
		LOCTEXT("FSPEditorEdModeName", "FSPEditorEdMode"), FSlateIcon(), true);
}

void FFSPEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FFSPEditorEdMode::EM_FSPEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFSPEditorModule, FSPEditor);