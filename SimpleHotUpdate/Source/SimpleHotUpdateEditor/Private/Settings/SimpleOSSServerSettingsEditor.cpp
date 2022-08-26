// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Settings/SimpleOSSServerSettingsEditor.h"

USimpleOSSServerSettingsEditor::USimpleOSSServerSettingsEditor()
{

}
#if WITH_EDITOR
void USimpleOSSServerSettingsEditor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName().ToString() == TEXT("Endpoint") || 
		PropertyChangedEvent.GetPropertyName().ToString() == TEXT("BucketName"))
	{
		ResetLinkURL();
	}
}
#endif

void USimpleOSSServerSettingsEditor::ResetLinkURL()
{
	LinkURL = *(BucketName.ToString() + TEXT(".") + Endpoint);
	UpdateStorageSettingsInstance(LinkURL.ToString());
}