// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleHotUpdateEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleHotUpdateEditorModule"

void FSimpleHotUpdateEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SimpleHotUpdateEditor", "Bring up SimpleHotUpdateEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
