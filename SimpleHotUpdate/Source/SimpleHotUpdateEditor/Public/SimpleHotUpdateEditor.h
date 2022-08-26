// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSimpleHotUpdateEditorModule : public IModuleInterface
{
	friend class SHotUpdateSlateMain;
	friend class USimpleUnrealPakSettingsEditor;
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

private:
	//鼠标右键
	TSharedRef<FExtender> OnExtendContentAssetBrowser(const TArray<FAssetData>& AssetData);

	TSharedRef<FExtender> OnExtendContentBrowser(const TArray<FString>& NewPaths);
	void CreateSuMenuForContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSubMenuType InType);
	void SubExtendContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSubMenuType InType);
	
	void UnitTesting(TArray<FString> NewPaths);

	void Push(TArray<FString> NewPaths);
	void Push(TArray<FString> NewPaths, ESimpleSubMenuType InType);
	void Push(EObjectStorageServerType ServerType,const UObjectStorageSettings *InStorageSettings);
	void Push(EObjectStorageServerType ServerType,const FString &InNewVersion);//单独上传版本
	void Unlock(EObjectStorageServerType ServerTyp);

	void VersionIteration();
	void VersionNumberRecursion(TArray<int32> &VersionNumber,int32 DepthValue);
private:

	void RegisterMenus();
	void ClearPak();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
