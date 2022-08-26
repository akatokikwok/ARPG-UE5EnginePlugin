// Copyright (C) RenZhai.2021.All Rights Reserved.

#include "SimpleHotPakBPLibrary.h"
#include "SimpleHotPak.h"
#include "IPlatformFilePak.h"
#include "SimpleHotPakMethod.hpp"
#include "Log/SimpleHotPakLog.h"
#include "Blueprint/UserWidget.h"

FSimpleHotPakConfig HotPakConfig;

FPakPlatformFile* USimpleHotPakBPLibrary::GetPakPlatformFile()
{
	FSimpleHotPakModule& SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleHotPakModule>(TEXT("SimpleHotPak"));
	return SimpleUnrealPakModule.GetPakPlatformFile();
}

void USimpleHotPakBPLibrary::InitConfig(const FSimpleHotPakConfig& InConfig)
{
	HotPakConfig = InConfig;
}

void USimpleHotPakBPLibrary::GetFilenamesInPakFile(const FString& InPakFilename, TArray<FString>& OutFileList)
{
	GetPakPlatformFile()->GetFilenamesInPakFile(InPakFilename, OutFileList);
}

bool USimpleHotPakBPLibrary::IsMounted(const FString& PakFilename)
{
	if (FPakPlatformFile* InPakFile = GetPakPlatformFile())
	{
		TArray<FString> PakFilenames;
		InPakFile->GetMountedPakFilenames(PakFilenames);

		return PakFilenames.Contains(PakFilename);
	}

	return false;
}

TArray<FString> USimpleHotPakBPLibrary::GetMountedReturnPakFilenames()
{
	if (FPakPlatformFile* InPakFile = GetPakPlatformFile())
	{
		TArray<FString> PakFilenames;
		InPakFile->GetMountedPakFilenames(PakFilenames);

		return PakFilenames;
	}

	return TArray<FString>();
}

void USimpleHotPakBPLibrary::GetMountedPakFilenames(TArray<FString>& OutPakFilenames)
{
	if (FPakPlatformFile* InPakFile = GetPakPlatformFile())
	{
		InPakFile->GetMountedPakFilenames(OutPakFilenames);
	}
}

bool USimpleHotPakBPLibrary::FindFile(const FString& Directory, TArray<FString>& File, bool bRecursively)
{
	class FSimplePakFindFile :public IPlatformFile::FDirectoryVisitor
	{
	public:
		FSimplePakFindFile(TArray<FString>& VisitFiles)
			:VisitFiles(VisitFiles)
		{}

		/**
		 * Callback for a single file or a directory in a directory iteration.
		 * @param FilenameOrDirectory		If bIsDirectory is true, this is a directory (with no trailing path delimiter), otherwise it is a file name.
		 * @param bIsDirectory				true if FilenameOrDirectory is a directory.
		 * @return							true if the iteration should continue.
		**/
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				VisitFiles.Add(FilenameOrDirectory);
			}

			return true;
		}

		TArray<FString>& VisitFiles;
	};

	FSimplePakFindFile Visitor(File);

	if (bRecursively)
	{
		return GetPakPlatformFile()->IterateDirectoryRecursively(*Directory, Visitor);
	}

	return GetPakPlatformFile()->IterateDirectory(*Directory, Visitor);
}

USimpleHotPakBPLibrary::USimpleHotPakBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool USimpleHotPakBPLibrary::MountPak(const FString& PakFilename, int32 PakOrder, const FString& MountPath, bool bSelfAdaptionMountProjectPath)
{
	if (FPakPlatformFile *InPakFile = GetPakPlatformFile())
	{
		if (!IsMounted(PakFilename))
		{
			//解密
			SimpleHotPak::AESDecrypt(HotPakConfig);

			//这个必须要这么做,因为我们希望不同的项目打包出来的pak依然可以安装到其他项目中
			FString NewMountPath = MountPath;
			if (NewMountPath.IsEmpty())
			{
				if (bSelfAdaptionMountProjectPath)
				{
#if (ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION == 2 && ENGINE_PATCH_VERSION > 6))
					TRefCountPtr<FPakFile> PakFile = new FPakFile(InPakFile, *PakFilename, false, true);
					if (PakFile.GetReference()->IsValid())
					{
						//重新设置安装路径
						NewMountPath = SimpleHotPak::GetShortMountPoint(PakFile->GetMountPoint());
					}

					PakFile.SafeRelease();

#else
					FPakFile *PakFile = new FPakFile(InPakFile, *PakFilename, false);
					if (PakFile->IsValid())
					{
						//重新设置安装路径
						NewMountPath = SimpleHotPak::GetShortMountPoint(PakFile->GetMountPoint());
					}

					delete PakFile;
#endif		
				}
			}
			
			return InPakFile->Mount(*PakFilename, PakOrder, NewMountPath.Len() > 0 ? *NewMountPath : NULL);
		}
	}

	return false;
}

bool USimpleHotPakBPLibrary::UnmountPak(const FString& PakFilename)
{
	if (FPakPlatformFile* InPakFile = GetPakPlatformFile())
	{
		return InPakFile->Unmount(*PakFilename);
	}

	return false;
}

UObject* USimpleHotPakBPLibrary::GetObjectFromPak(const FString& Filename)
{
	return SimpleHotPak::StaticLoadObjectFromPak<UObject>(Filename);;
}

UClass* USimpleHotPakBPLibrary::GetClassFromPak(const FString& Filename,const FString & InSuffix, const FString& InPrefix)
{
	return SimpleHotPak::StaticLoadObjectFromPak<UClass>(Filename,
		(!InSuffix.IsEmpty() ? *InSuffix : NULL),
		(!InPrefix.IsEmpty() ? *InPrefix : NULL));
}

UMaterial* USimpleHotPakBPLibrary::GetMaterialFromPak(const FString& Filename)
{
	return Cast<UMaterial>(GetObjectFromPak(Filename));
}

UTexture2D* USimpleHotPakBPLibrary::GetTexture2DFromPak(const FString& Filename)
{
	return Cast<UTexture2D>(GetObjectFromPak(Filename));
}

USkeletalMesh* USimpleHotPakBPLibrary::GetSkeletalMeshFromPak(const FString& Filename)
{
	return Cast<USkeletalMesh>(GetObjectFromPak(Filename));
}

UStaticMesh* USimpleHotPakBPLibrary::GetStaticMeshFromPak(const FString& Filename)
{
	return Cast<UStaticMesh>(GetObjectFromPak(Filename));
}

AActor* USimpleHotPakBPLibrary::GetActorFromPak(UObject* WorldContextObject, const FString& Filename, const FVector& InLocation, const FRotator& InRotator)
{
	UWorld* World = Cast<UWorld>(WorldContextObject);
	if (!World)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (World)
	{
		if (UClass* InGeneratedClass = GetClassFromPak(Filename,TEXT("_C"), TEXT("Blueprint")))
		{
			return World->SpawnActor<AActor>(InGeneratedClass, InLocation, InRotator);
		}
	}

	return nullptr;
}

UUserWidget* USimpleHotPakBPLibrary::GetUMGFromPak(UObject* WorldContextObject, const FString& Filename)
{
	UWorld* World = Cast<UWorld>(WorldContextObject);
	if (!World)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (World)
	{
		if (TSubclassOf<UUserWidget> InGeneratedClass = GetClassFromPak(Filename,TEXT("_C"),TEXT("WidgetBlueprint")))
		{
			return CreateWidget<UUserWidget>(World,InGeneratedClass);
		}
	}

	return nullptr;
}

