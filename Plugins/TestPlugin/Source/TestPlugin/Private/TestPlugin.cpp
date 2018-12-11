// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TestPlugin.h"
#include "TestPluginEdMode.h"

#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FTestPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FTestPluginEdMode>(FTestPluginEdMode::EM_TestPluginEdModeId, LOCTEXT("TestPluginEdModeName", "TestPluginEdMode"), FSlateIcon(), true);
}

void FTestPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FTestPluginEdMode::EM_TestPluginEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestPluginModule, TestPlugin)