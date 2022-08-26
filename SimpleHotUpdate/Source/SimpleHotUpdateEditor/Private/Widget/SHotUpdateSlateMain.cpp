// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "Widget/SHotUpdateSlateMain.h"
#include "Settings/ObjectStorageServerSettingsEditor.h"
#include "Settings/SimpleHTTPServerSettingsEditor.h"
#include "Settings/SimpleOSSServerSettingsEditor.h"
#include "Settings/SimpleUnrealPakSettingsEditor.h"
#include "Settings/SimpleLocalSettingsEditor.h"
#include "Settings/ObjectStorageSettings.h"
#include "Widgets/Layout/SBox.h"
#include "PropertyEditorModule.h"
#include "Widgets/Layout/SScrollBox.h"
#include "SimpleHotUpdateEditor.h"
#include "..\..\Public\Widget\SHotUpdateSlateMain.h"

#define LOCTEXT_NAMESPACE "SHotUpdateSlateMain"

void SHotUpdateSlateMain::Construct(const FArguments& InArgs)
{
	FMenuBuilder PutServetModeBuilder(true, NULL);
	FUIAction HTTPServer(FExecuteAction::CreateSP(this,&SHotUpdateSlateMain::HandlePutServetTypeByClicked, EObjectStorageServerType::HTTP_SERVER));
	PutServetModeBuilder.AddMenuEntry(LOCTEXT("HTTP_Server", "HTTPServer"), LOCTEXT("HTTP_ServerTip", "You can set HTTP server related content."), FSlateIcon(),HTTPServer);
	
	FUIAction OSSServer(FExecuteAction::CreateSP(this, &SHotUpdateSlateMain::HandlePutServetTypeByClicked, EObjectStorageServerType::OSS_SERVER));
	PutServetModeBuilder.AddMenuEntry(LOCTEXT("OSS_SERVER", "OSSServer"), LOCTEXT("OSSServerTip", "Alibaba cloud based object storage server."), FSlateIcon(), OSSServer);
	
	FUIAction LocalVerison(FExecuteAction::CreateSP(this, &SHotUpdateSlateMain::HandlePutServetTypeByClicked, EObjectStorageServerType::LOCAL_POS));
	PutServetModeBuilder.AddMenuEntry(LOCTEXT("LOCAL_VERISON", "Local"), LOCTEXT("LocalVerisonTip", "Store verison to the specified local location."), FSlateIcon(), LocalVerison);
	
	ChildSlot
	[
		SNew(SScrollBox)
		.Orientation(Orient_Vertical)
		+ SScrollBox::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.Padding(4.f, 2.f, 4.f, 2.f)
				[
					SNew(SComboButton)
					.ButtonContent()
					[
						SNew(STextBlock)
						.Text(this, &SHotUpdateSlateMain::PutServetModeBuilderText)
					]
					.MenuContent()
					[
						PutServetModeBuilder.MakeWidget()
					]
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.Padding(4.f, 2.f, 4.f, 2.f)
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Get_Current_Version", "Current Version"))
					.HAlign(HAlign_Center)
					.OnClicked(this, &SHotUpdateSlateMain::GetCurrentVersion)
					.ToolTipText(LOCTEXT("Get_Current_VersionTip", "Get current version."))
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.Padding(4.f, 2.f, 4.f, 2.f)
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Get_Unlock", "Unlock"))
					.HAlign(HAlign_Center)
					.OnClicked(this, &SHotUpdateSlateMain::Unlock)
					.ToolTipText(LOCTEXT("UnlockTip", "Unlock server deadlock."))
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.Padding(4.f, 2.f, 4.f, 2.f)
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Get_Push_Version", "Push Version Info"))
					.HAlign(HAlign_Center)
					.OnClicked(this, &SHotUpdateSlateMain::PushVersion)
					.IsEnabled(this, &SHotUpdateSlateMain::IsPushVersion)
					.ToolTipText(LOCTEXT("Get_Push_VersionTip", "Only upload version information, excluding resources. It is generally set for discarding resources."))
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.Padding(4.f, 2.f, 4.f, 2.f)
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Save_as_Default", "Save as Default"))
					.HAlign(HAlign_Center)
					.OnClicked(this, &SHotUpdateSlateMain::SaveAsDefault)
					.ToolTipText(LOCTEXT("Save_as_DefaultTip", "Store configuration locally."))
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.Padding(4.f, 2.f, 4.f, 2.f)
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Upload_Custom_File", "Push To Server"))
					.HAlign(HAlign_Center)
					.IsEnabled(this, &SHotUpdateSlateMain::IsPutToServer)
					.OnClicked(this, &SHotUpdateSlateMain::PutToServer)
					.ToolTipText(LOCTEXT("Upload_Custom_File_DefaultTip", "As a hot update upload button, it can be displayed correctly as long as the configuration is correct."))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(ObjectContent, SBox)
			]
		]
	];

	InitView();
	InitConfig();

	SaveConfig();
}

template<class T>
void SaveObjectConfig()
{
	if (T* InSettingsEditor = const_cast<T*>(GetDefault<T>()))
	{
		InSettingsEditor->SaveObjectStorageConfig();
	}
}

template<class T>
void LoadObjectConfig()
{
	if (T* InSettingsEditor = const_cast<T*>(GetDefault<T>()))
	{
		InSettingsEditor->LoadObjectStorageConfig();
	}
}

bool SHotUpdateSlateMain::IsPutToServer()const
{
	if (const UObjectStorageSettings *InObjectStorageSettings = GetStorageSettings())
	{
		if (InObjectStorageSettings->CustomExtraInstallationInfo.Num() > 0)
		{
			return !InObjectStorageSettings->CustomExtraInstallationInfo[0].FilePath.FilePath.IsEmpty();
		}
		else if(InObjectStorageSettings->MainVersionInstallationInfo.Num() > 0)
		{
			return !InObjectStorageSettings->MainVersionInstallationInfo[0].FilePath.FilePath.IsEmpty();
		}
	}

	return false;
}

bool SHotUpdateSlateMain::IsPushVersion() const
{
	return GetDefault<USimpleUnrealPakSettingsEditor>()->VersionControlObject != NULL && 
		GetDefault<USimpleUnrealPakSettingsEditor>()->VersionDiscardSettings.Num() > 0;
}

FReply SHotUpdateSlateMain::PutToServer()
{
	if (IsPutToServer())
	{
		if (FSimpleHotUpdateEditorModule* InEditorModule = &FModuleManager::LoadModuleChecked<FSimpleHotUpdateEditorModule>(TEXT("SimpleHotUpdateEditor")))
		{
			InEditorModule->Push(
				GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType,
				GetStorageSettings());
		}
	}

	return FReply::Handled();
}

FReply SHotUpdateSlateMain::PushVersion()
{
	if (USimpleUnrealPakSettingsEditor* InUnrealPakSettingsEditor = const_cast<USimpleUnrealPakSettingsEditor*>(GetDefault<USimpleUnrealPakSettingsEditor>()))
	{
		InUnrealPakSettingsEditor->UploadDiscardedVersion();
	}

	return FReply::Handled();
}

FReply SHotUpdateSlateMain::GetCurrentVersion()
{
	if (USimpleUnrealPakSettingsEditor* InUnrealPakSettingsEditor = const_cast<USimpleUnrealPakSettingsEditor*>(GetDefault<USimpleUnrealPakSettingsEditor>()))
	{
		InUnrealPakSettingsEditor->BuildVersionToDiscard();
	}

	return FReply::Handled();
}

FReply SHotUpdateSlateMain::SaveAsDefault()
{
	if (FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PreSaveAsDefaultUserData", 
		"Are you sure you want to store the above data in the local configuration?")) != EAppReturnType::Yes)
	{
		return FReply::Unhandled();
	}

	SaveConfig();

	FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PostSaveAsDefaultUserData",
		"The data of the object has been stored."));

	return FReply::Handled();
}

void SHotUpdateSlateMain::InitConfig()
{
	LoadObjectConfig<USimpleHTTPServerSettingsEditor>();
	LoadObjectConfig<USimpleOSSServerSettingsEditor>();
	LoadObjectConfig<USimpleLocalSettingsEditor>();
	LoadObjectConfig<USimpleUnrealPakSettingsEditor>();
	LoadObjectConfig<UObjectStorageServerSettingsEditor>();
	LoadObjectConfig<UObjectStorageSettings>();
}

FReply SHotUpdateSlateMain::Unlock()
{
	if (FSimpleHotUpdateEditorModule* InEditorModule = &FModuleManager::LoadModuleChecked<FSimpleHotUpdateEditorModule>(TEXT("SimpleHotUpdateEditor")))
	{
		InEditorModule->Unlock(GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType);
	}

	return FReply::Handled();
}

void SHotUpdateSlateMain::SaveConfig()
{
	switch (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType)
	{
	case EObjectStorageServerType::HTTP_SERVER:
	{
		SaveObjectConfig<USimpleHTTPServerSettingsEditor>();
		if (USimpleHTTPServerSettingsEditor* InSettingsEditor = const_cast<USimpleHTTPServerSettingsEditor*>(GetDefault<USimpleHTTPServerSettingsEditor>()))
		{
			InSettingsEditor->ResetLinkURL();
		}
		break;
	}
	case EObjectStorageServerType::OSS_SERVER:
	{
		SaveObjectConfig<USimpleOSSServerSettingsEditor>();
		if (USimpleOSSServerSettingsEditor* InSettingsEditor = const_cast<USimpleOSSServerSettingsEditor*>(GetDefault<USimpleOSSServerSettingsEditor>()))
		{
			InSettingsEditor->ResetLinkURL();
		}
		break;
	}case EObjectStorageServerType::LOCAL_POS:
	{
		SaveObjectConfig<USimpleLocalSettingsEditor>();
		if (USimpleLocalSettingsEditor* InSettingsEditor = const_cast<USimpleLocalSettingsEditor*>(GetDefault<USimpleLocalSettingsEditor>()))
		{
			InSettingsEditor->ResetLinkURL();
		}
		break;
	}
	}

	SaveObjectConfig<USimpleUnrealPakSettingsEditor>();
	SaveObjectConfig<UObjectStorageServerSettingsEditor>();
}

FText SHotUpdateSlateMain::PutServetModeBuilderText() const
{
	if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::HTTP_SERVER)
	{
		return LOCTEXT("HTTP_Server", "HTTPServer");
	}
	else if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::OSS_SERVER)
	{
		return LOCTEXT("OSS_SERVER", "OSSServer");
	}
	else if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::LOCAL_POS)
	{
		return LOCTEXT("LOCAL_POS", "Local");
	}

	return LOCTEXT("Server_None", "Server_None");
}

void SHotUpdateSlateMain::InitView()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowMultipleTopLevelObjects = true;
		DetailsViewArgs.bShowPropertyMatrixButton = false;
	}
	TSharedPtr<class IDetailsView> ConfigPanel = PropertyModule.CreateDetailView(DetailsViewArgs);

	TArray<UObject*> SourceObjects;
	SourceObjects.Reserve(2);
	if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::HTTP_SERVER)
	{
		SourceObjects.Add(const_cast<USimpleHTTPServerSettingsEditor*>(GetDefault<USimpleHTTPServerSettingsEditor>()));
	}
	else if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::OSS_SERVER)
	{
		SourceObjects.Add(const_cast<USimpleOSSServerSettingsEditor*>(GetDefault<USimpleOSSServerSettingsEditor>()));
	}
	else if (GetDefault<UObjectStorageServerSettingsEditor>()->ObjectStorageServerType == EObjectStorageServerType::LOCAL_POS)
	{
		SourceObjects.Add(const_cast<USimpleLocalSettingsEditor*>(GetDefault<USimpleLocalSettingsEditor>()));
	}

	SourceObjects.Add(const_cast<USimpleUnrealPakSettingsEditor*>(GetDefault<USimpleUnrealPakSettingsEditor>()));

	ConfigPanel->SetObjects(SourceObjects);
	ObjectContent->SetContent(ConfigPanel.ToSharedRef());
}

void SHotUpdateSlateMain::HandlePutServetTypeByClicked(EObjectStorageServerType InOSType)
{
	if (const UObjectStorageServerSettingsEditor* ServerSettingsEditorConst = GetDefault<UObjectStorageServerSettingsEditor>())
	{
		UObjectStorageServerSettingsEditor* ServerSettingsEditor = const_cast<UObjectStorageServerSettingsEditor*>(ServerSettingsEditorConst);
		ServerSettingsEditor->ObjectStorageServerType = InOSType;

		InitView();

		SaveConfig();
	}
}

#undef LOCTEXT_NAMESPACE