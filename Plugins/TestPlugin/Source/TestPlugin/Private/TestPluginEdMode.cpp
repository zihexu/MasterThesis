// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TestPluginEdMode.h"
#include "TestPluginEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FTestPluginEdMode::EM_TestPluginEdModeId = TEXT("EM_TestPluginEdMode");

FTestPluginEdMode::FTestPluginEdMode()
{

}

FTestPluginEdMode::~FTestPluginEdMode()
{

}

void FTestPluginEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FTestPluginEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FTestPluginEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FTestPluginEdMode::UsesToolkits() const
{
	return true;
}




