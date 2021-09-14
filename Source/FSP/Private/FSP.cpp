// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FSP.h"

#define LOCTEXT_NAMESPACE "FFSPModule"

void FFSPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFSPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

DEFINE_LOG_CATEGORY(LogFSP);

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFSPModule, FSP)