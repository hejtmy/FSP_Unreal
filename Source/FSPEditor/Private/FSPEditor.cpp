# include "FSPEditor.h"

#define LOCTEXT_NAMESPACE "FFSPEditorModule"

void FFSPEditorModule::StartupModule()
{

}

void FFSPEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFSPEditorModule, FSPEditor)