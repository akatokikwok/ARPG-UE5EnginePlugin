// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Settings/SimpleHTTPServerSettingsEditor.h"
#include "Settings/ObjectStorageSettings.h"

USimpleHTTPServerSettingsEditor::USimpleHTTPServerSettingsEditor()
{

}
#if WITH_EDITOR
void USimpleHTTPServerSettingsEditor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName().ToString() == TEXT("PublicIP") || 
		PropertyChangedEvent.GetPropertyName().ToString() == TEXT("LANIP"))
	{
		ResetLinkURL();
	}
}
#endif

void USimpleHTTPServerSettingsEditor::ResetLinkURL()
{
	LinkURL = *(PublicIP / ObjectStorageName.ToString());

	UpdateStorageSettingsInstance(LinkURL.ToString());
}