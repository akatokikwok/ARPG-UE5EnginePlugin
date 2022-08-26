// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SimpleHotUpdateEditorStyle.h"

class FSimpleHotUpdateEditorCommands : public TCommands<FSimpleHotUpdateEditorCommands>
{
public:

	FSimpleHotUpdateEditorCommands()
		: TCommands<FSimpleHotUpdateEditorCommands>(TEXT("SimpleHotUpdateEditor"), NSLOCTEXT("Contexts", "SimpleHotUpdateEditor", "SimpleHotUpdateEditor Plugin"), NAME_None, FSimpleHotUpdateEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};