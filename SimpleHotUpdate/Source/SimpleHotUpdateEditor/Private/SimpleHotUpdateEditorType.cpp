#include "SimpleHotUpdateEditorType.h"

const UObjectStorageSettings* GetStorageSettings()
{
	switch (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType)
	{
		case EObjectStorageServerType::HTTP_SERVER:
		{
			return GetDefault<USimpleHTTPServerSettingsEditor>();
		}
		case EObjectStorageServerType::OSS_SERVER:
		{
			return GetDefault<USimpleOSSServerSettingsEditor>();
		}
		case EObjectStorageServerType::LOCAL_POS:
		{
			return GetDefault<USimpleLocalSettingsEditor>();
		}
	}

	return NULL;
}
