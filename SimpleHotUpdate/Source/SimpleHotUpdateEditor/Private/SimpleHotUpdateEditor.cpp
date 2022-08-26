// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleHotUpdateEditor.h"
#include "SimpleHotUpdateEditorStyle.h"
#include "SimpleHotUpdateEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "ContentBrowserModule.h"
#include "Pak/UnrealPakFile.h"
#include "ISettingsModule.h"
#include "Settings/SimpleUnrealPakSettingsEditor.h"
#include "Settings/SimpleHTTPServerSettingsEditor.h"
#include "Settings/SimpleOSSServerSettingsEditor.h"
#include "Settings/SimpleLocalSettingsEditor.h"
#include "Settings/ObjectStorageServerSettingsEditor.h"
#include "Widget/SHotUpdateSlateMain.h"
#include "ObjectStorageServer/ObjectStorageServer.h"
#include "HAL/IPlatformFileOpenLogWrapper.h"

static const FName SimpleHotUpdateEditorTabName("SimpleHotUpdateEditor");

#define LOCTEXT_NAMESPACE "FSimpleHotUpdateEditorModule"

void FSimpleHotUpdateEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleHotUpdateEditorStyle::Initialize();
	FSimpleHotUpdateEditorStyle::ReloadTextures();

	FSimpleHotUpdateEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimpleHotUpdateEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSimpleHotUpdateEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSimpleHotUpdateEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SimpleHotUpdateEditorTabName, FOnSpawnTab::CreateRaw(this, &FSimpleHotUpdateEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSimpleHotUpdateEditorTabTitle", "SimpleHotUpdateEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	//文件夹
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserMenuExtender_SelectedPaths = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
	ContentBrowserMenuExtender_SelectedPaths.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this,&FSimpleHotUpdateEditorModule::OnExtendContentBrowser));

	//文件
	TArray<FContentBrowserMenuExtender_SelectedAssets>& ContentBrowserMenuExtender_SelectedAssets = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	ContentBrowserMenuExtender_SelectedAssets.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FSimpleHotUpdateEditorModule::OnExtendContentAssetBrowser));

	//注册设置
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		//注册pak
		SettingsModule->RegisterSettings("Project", "ProjectPak", "PakSettings",
			LOCTEXT("SimpleUnrealPakSettings", "Pak Settings"), 
			LOCTEXT("SimpleUnrealPakSettingsTip", "Package settings for the current platform you have set."),
			GetMutableDefault<USimpleUnrealPakSettingsEditor>());

		//对象存储总设置
		SettingsModule->RegisterSettings("Project", "ObjectStorage", "ObjectStorageTotalSettings",
			LOCTEXT("ObjectStorageSettings", "A Object Storage Total Settings"),
			LOCTEXT("ObjectStorageSettingsTip", "Total settings for the object storage server."),
			GetMutableDefault<UObjectStorageServerSettingsEditor>());

		//HTTP设置
		SettingsModule->RegisterSettings("Project", "ObjectStorage", "HTTPSettings",
			LOCTEXT("HTTPServerSettings", "HTTP Server Settings"),
			LOCTEXT("HTTPServerSettingsTip", "Support custom object storage server."),
			GetMutableDefault<USimpleHTTPServerSettingsEditor>());

		//OSS设置
		SettingsModule->RegisterSettings("Project", "ObjectStorage", "OSSSettings",
			LOCTEXT("OSSServerSettings", "OSS Server Settings"),
			LOCTEXT("OSSServerSettingsTip", "Support Alibaba cloud object storage server."),
			GetMutableDefault<USimpleOSSServerSettingsEditor>());

		//Local设置
		SettingsModule->RegisterSettings("Project", "ObjectStorage", "LocalVersionSettings",
			LOCTEXT("LocalVersionSettings", "Local Version Settings"),
			LOCTEXT("LocalVersionSettingsTip", "Support Alibaba Local object storage Version."),
			GetMutableDefault<USimpleLocalSettingsEditor>());
	}
}

void FSimpleHotUpdateEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSimpleHotUpdateEditorStyle::Shutdown();

	FSimpleHotUpdateEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SimpleHotUpdateEditorTabName);
}

TSharedRef<SDockTab> FSimpleHotUpdateEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SHotUpdateSlateMain)
			]
		];
}

void FSimpleHotUpdateEditorModule::PluginButtonClicked()
{
#if (ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION == 2 && ENGINE_PATCH_VERSION >= 7))
	FGlobalTabmanager::Get()->TryInvokeTab(SimpleHotUpdateEditorTabName);
#else
	FGlobalTabmanager::Get()->InvokeTab(FTabId(SimpleHotUpdateEditorTabName));
#endif
}

TSharedRef<FExtender> FSimpleHotUpdateEditorModule::OnExtendContentAssetBrowser(const TArray<FAssetData>& AssetData)
{
	TSharedRef<FExtender> Extender(new FExtender);
	TArray<FString> NewPaths;
	for (const auto& Tmp : AssetData)
	{
		NewPaths.Add(Tmp.PackageName.ToString());
	}
	Extender->AddMenuExtension(
		"AssetContextReferences",
		EExtensionHook::Before, 
		nullptr, 
		FMenuExtensionDelegate::CreateRaw(this,
			&FSimpleHotUpdateEditorModule::CreateSuMenuForContentBrowser,
			NewPaths,ESimpleSubMenuType::HU_ASSET));

	return Extender;
}

TSharedRef<FExtender> FSimpleHotUpdateEditorModule::OnExtendContentBrowser(const TArray<FString>& NewPaths)
{
	TSharedRef<FExtender> Extender(new FExtender);

	Extender->AddMenuExtension("FolderContext", EExtensionHook::Before,NULL, FMenuExtensionDelegate::CreateRaw(
		this,
		&FSimpleHotUpdateEditorModule::CreateSuMenuForContentBrowser, NewPaths, ESimpleSubMenuType::HU_CONTENT));

	return Extender;
}

void FSimpleHotUpdateEditorModule::CreateSuMenuForContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSubMenuType InType)
{
	FText SectionName;
	FText SectionNameTip;
	switch (InType)
	{
		case ESimpleSubMenuType::HU_CONTENT:
		{
			SectionName = LOCTEXT("HotUpdateContentEditor", "Package resources");
			SectionNameTip = LOCTEXT("HotUpdateContentEditorDescribe", "Upload data to the server .");
		
			break;
		}
		case ESimpleSubMenuType::HU_ASSET:
		{
			SectionName = LOCTEXT("HotUpdateContentAssetEditor", "Package Asset to Server");
			SectionNameTip = LOCTEXT("HotUpdateContentEditorAssetDescribe", "Upload data to the server .");

			break;
		}
	}

	MunuBuilder.AddSubMenu(
		SectionName,
		SectionNameTip,
		FNewMenuDelegate::CreateRaw(this, 
			&FSimpleHotUpdateEditorModule::SubExtendContentBrowser,
			NewPaths,
			InType));
}

void FSimpleHotUpdateEditorModule::SubExtendContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSubMenuType InType)
{
	MunuBuilder.BeginSection("HotUpdate", LOCTEXT("HotUpdateMenu", "Upload server mode"));

	FText SectionName;
	FText SectionNameTip;
	switch (InType)
	{
		case ESimpleSubMenuType::HU_CONTENT:
		{
			if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::LOCAL_POS)
			{
				SectionName = LOCTEXT("HotUpdate_PushLocal", "Push To Local");
				SectionNameTip = LOCTEXT("HotUpdate_PushTip", "Push resources to the local.");
			}
			else
			{
				SectionName = LOCTEXT("HotUpdate_PushServer", "Push To Sever");
				SectionNameTip = LOCTEXT("HotUpdate_PushTip", "Push resources to the Local.");
			}
			
			break;
		}
		case ESimpleSubMenuType::HU_ASSET:
		{
			if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::LOCAL_POS)
			{
				SectionName = LOCTEXT("HotUpdateAsset_Push", "Push Asset to local");
				SectionNameTip = LOCTEXT("HotUpdateAsset_PushTip", "Push resources to the local.");
			}
			else
			{
				SectionName = LOCTEXT("HotUpdateAsset_Push", "Push Asset");
				SectionNameTip = LOCTEXT("HotUpdateAsset_PushTip", "Push resources to the server.");
			}
			
			break;
		}
	}

	MunuBuilder.AddMenuEntry(
		SectionName,
		SectionNameTip,
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FSimpleHotUpdateEditorModule::Push,NewPaths, InType)));

	MunuBuilder.AddMenuEntry(
		LOCTEXT("HotUpdate_Pak", "Pak To Loacl"),
		SectionNameTip = LOCTEXT("HotUpdate_PakTip", "Pack resources as Pak to local."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FSimpleHotUpdateEditorModule::Push, NewPaths)));

	MunuBuilder.AddMenuEntry(
		LOCTEXT("HotUpdate_UnitTesting", "Unit Testing"),
		SectionNameTip = LOCTEXT("HotUpdate_UnitTestingTip", "This function is only used for unit testing."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FSimpleHotUpdateEditorModule::UnitTesting, NewPaths)));


	MunuBuilder.EndSection();
}

void FSimpleHotUpdateEditorModule::UnitTesting(TArray<FString> NewPaths)
{
	//测试-版本名字迭代功能
//	VersionIteration();
}

void FSimpleHotUpdateEditorModule::VersionNumberRecursion(TArray<int32>& VersionNumber, int32 DepthValue)
{
	int32 Number = VersionNumber.Num();
	if (Number >= DepthValue)
	{
		int32& LastNumber = VersionNumber[Number - DepthValue];
		if (LastNumber >= GetDefault<USimpleUnrealPakSettingsEditor>()->VersionMaximumBase)
		{
			VersionNumberRecursion(VersionNumber, ++DepthValue);

			//置为零
			LastNumber = 0;
		}
		else
		{
			LastNumber++;
		}
	}
}

void FSimpleHotUpdateEditorModule::Push(EObjectStorageServerType ServerType, const UObjectStorageSettings* InStorageSettings)
{
	if (InStorageSettings)
	{
		//版本名字迭代
		VersionIteration();

		auto UpdateToServer = [&](const TArray<FCustomVersionInstallation>& InArray,bool bMainVersion = false)
		{
			switch (ServerType)
			{
				case EObjectStorageServerType::HTTP_SERVER:
				{
					for (auto &Tmp : InArray)
					{
						FString RelativeInstallationLocation = Tmp.bInstallContentPak == false ?
							Tmp.RelativeInstallationLocation : TEXT("");

						SimpleUnrealPakEditor::FObjectStorageClient::HTTPPut(
							RelativeInstallationLocation,
							bMainVersion,
							Tmp.FilePath.FilePath);
					}
					
					break;
				}
				case EObjectStorageServerType::OSS_SERVER:
				{
					for (auto& Tmp : InArray)
					{
						FString RelativeInstallationLocation = Tmp.bInstallContentPak == false ?
							Tmp.RelativeInstallationLocation : TEXT("");

						SimpleUnrealPakEditor::FObjectStorageClient::OSSPut(
							RelativeInstallationLocation,
							bMainVersion,
							Tmp.FilePath.FilePath);
					}

					break;
				}
				case EObjectStorageServerType::LOCAL_POS:
				{
					for (auto& Tmp : InArray)
					{
						FString RelativeInstallationLocation = Tmp.bInstallContentPak == false ?
							Tmp.RelativeInstallationLocation : TEXT("");

						SimpleUnrealPakEditor::FObjectStorageClient::LocalPut(
							RelativeInstallationLocation,
							bMainVersion,
							Tmp.FilePath.FilePath);
					}

					break;
				}
			}
		};

		UpdateToServer(InStorageSettings->CustomExtraInstallationInfo);
		UpdateToServer(InStorageSettings->MainVersionInstallationInfo,true);

		FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PutMainPakSuccess",
			"The main version or additional version was uploaded successfully."));
	}
}

void FSimpleHotUpdateEditorModule::Push(EObjectStorageServerType ServerType,const FString& InNewVersion)
{
	switch (ServerType)
	{
		case EObjectStorageServerType::HTTP_SERVER:
		{
			SimpleUnrealPakEditor::FObjectStorageClient::HTTPPutVersion(InNewVersion);

			break;
		}
		case EObjectStorageServerType::OSS_SERVER:
		{
			SimpleUnrealPakEditor::FObjectStorageClient::OSSPutVersion(InNewVersion);

			break;
		}
		case EObjectStorageServerType::LOCAL_POS:
		{
			SimpleUnrealPakEditor::FObjectStorageClient::LocalPutVersion(InNewVersion);

			break;
		}
	}
}

void FSimpleHotUpdateEditorModule::Unlock(EObjectStorageServerType ServerTyp)
{
	switch (ServerTyp)
	{
	case EObjectStorageServerType::HTTP_SERVER:
		SimpleUnrealPakEditor::FObjectStorageClient::HTTPUnlock();
		break;
	case EObjectStorageServerType::OSS_SERVER:
		SimpleUnrealPakEditor::FObjectStorageClient::OSSUnlock();
		break;
	case EObjectStorageServerType::LOCAL_POS:
		SimpleUnrealPakEditor::FObjectStorageClient::LocalUnlock();
		break;
	}
}

void FSimpleHotUpdateEditorModule::VersionIteration()
{
	if (USimpleUnrealPakSettingsEditor* InSettingsEditor = const_cast<USimpleUnrealPakSettingsEditor*>(GetDefault<USimpleUnrealPakSettingsEditor>()))
	{
		FString VersionString = InSettingsEditor->VersionName.ToString();
		
		//I 将版本拆分
		TArray<FString> VersionArray;
		VersionString.ParseIntoArray(VersionArray, TEXT("."));

		//II 将拆分版本转为int32
		//xx.xx.xx
		TArray<int32> VersionArrayNumber;
		for (auto &Tmp : VersionArray)
		{
			VersionArrayNumber.Add(FCString::Atoi(*Tmp));
		}

		//III 版本的迭代运算
		VersionNumberRecursion(VersionArrayNumber,1);

		//V 版本合成
		FString NewVersion;
		for (auto &Tmp : VersionArrayNumber)
		{
			NewVersion += TEXT(".") + FString::FromInt(Tmp);
		}
		NewVersion.RemoveFromStart(".");

		//VI 版本复制
		InSettingsEditor->VersionName = *NewVersion;

		//保存
		InSettingsEditor->SaveObjectStorageConfig();
	}
}

void FSimpleHotUpdateEditorModule::Push(TArray<FString> NewPaths)
{	
	if (SimpleUnrealPakEditor::FPak().PakFile(NewPaths))
	{
		FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PakLocalSuccess",
			"The resource is packaged successfully and has been stored in the local."));

		FPlatformProcess::ExploreFolder(*GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path);

		//检查打包的pak项目
		for (auto &Tmp : NewPaths)
		{
			FString PakName = *GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path / FPaths::GetCleanFilename(Tmp) + TEXT(".pak");
			SimpleUnrealPakEditor::FPak().ListPak(PakName);

			//if (FPlatformFileOpenLog* PlatformFileOpenLog = (FPlatformFileOpenLog*)(FPlatformFileManager::Get().FindPlatformFile(FPlatformFileOpenLog::GetTypeName())))
			//{
			//	PlatformFileOpenLog->AddPackageToOpenLog(*PakName);
			//}
		}
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PakLocalFail",
			"Resource packaging failed. Check whether the resource has been cooked or whether the resource exists."));
	}
}

void FSimpleHotUpdateEditorModule::Push(TArray<FString> NewPaths, ESimpleSubMenuType InType)
{
	//先清除前任的pak痕迹
	ClearPak();

	//再继续pak资源
	bool bPak = false;
	switch (InType)
	{
	case ESimpleSubMenuType::HU_CONTENT:
		bPak = SimpleUnrealPakEditor::FPak().PakFile(NewPaths);
		break;
	case ESimpleSubMenuType::HU_ASSET:
		bPak = SimpleUnrealPakEditor::FPak().PakMapFile(NewPaths);
		break;
	}

	if (bPak)
	{
		if (const UObjectStorageSettings* InStorageSettings = GetStorageSettings())
		{
			FString RelativeInstallationLocation = InStorageSettings->bInstallContentPak == false ?
				InStorageSettings->RelativeInstallationLocation : TEXT("");

			//版本名字迭代
			VersionIteration();

			//推到对象存储服务器
			if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::HTTP_SERVER)
			{
				SimpleUnrealPakEditor::FObjectStorageClient::HTTPPut(RelativeInstallationLocation);
			}
			else if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::OSS_SERVER)
			{
				SimpleUnrealPakEditor::FObjectStorageClient::OSSPut(RelativeInstallationLocation);
			}
			else if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::LOCAL_POS)
			{
				SimpleUnrealPakEditor::FObjectStorageClient::LocalPut(RelativeInstallationLocation);
			}
		}

		if (GetDefault<UObjectStorageServerSettingsEditor>()->bClearPak)
		{
			//清除Pak
			ClearPak();
		}

		FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PutPakSuccess",
			"Resources uploaded successfully."));
	}
	else
	{
		check(0);
	}
}

void FSimpleHotUpdateEditorModule::ClearPak()
{
	const FString PakPath = GetDefault<USimpleUnrealPakSettingsEditor>()->PakSavePaths.Path;

	TArray<FString> PakLocalPaths;
	IFileManager::Get().FindFilesRecursive(PakLocalPaths, *PakPath, TEXT("*"), true, false);
	for (auto &Tmp : PakLocalPaths)
	{
		if (IFileManager::Get().Delete(*Tmp))
		{

		}
	}
}

void FSimpleHotUpdateEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSimpleHotUpdateEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSimpleHotUpdateEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleHotUpdateEditorModule, SimpleHotUpdateEditor)