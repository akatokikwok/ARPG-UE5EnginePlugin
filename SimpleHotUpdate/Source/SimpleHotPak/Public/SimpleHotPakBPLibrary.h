// Copyright (C) RenZhai.2021.All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleHotPakType.h"
#include "SimpleHotPakBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
class UUserWidget;
class FPakPlatformFile;
UCLASS()
class SIMPLEHOTPAK_API USimpleHotPakBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static FPakPlatformFile* GetPakPlatformFile();

public:
	/*
	* Initialize our configuration, which is related to our AES encryption.
	* @InConfig       It's about encrypted AES.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPakInfo", Keywords = "IFO"), Category = "SimpleHotUpdate")
	static void InitConfig(const FSimpleHotPakConfig& InConfig);
	
	/*
	* Initialize our configuration, which is related to our AES encryption.
	* @InPakFilename According to the installation path of Pak, such as "../../yourpath/xxx.pak".
	* @OutFileList   Get the resource image path inside the Pak .
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPakInfo", Keywords = "IFO"), Category = "SimpleHotUpdate")
	static void GetFilenamesInPakFile(const FString& InPakFilename, TArray<FString>& OutFileList);

	/*
	* Has the package been installed.
	* @PakFilename	  Name of the package.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPakInfo", Keywords = "IFO"), Category = "SimpleHotUpdate")
	static bool IsMounted(const FString& PakFilename);

	/*
	* You can get the names of all pak that have been installed so far .
	* @Return		Returns the name of the installation package.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetMountedFilenames", Keywords = "PakInfo"), Category = "SimpleHotUpdate")
	static TArray<FString> GetMountedReturnPakFilenames();

	/*
	* You can get the names of all pak that have been installed so far .
	* @Return		Returns the name of the installation package.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetMountedFilenames", Keywords = "PakInfo"), Category = "SimpleHotUpdate")
	static void GetMountedPakFilenames(TArray<FString> &OutPakFilenames);

	/*
	* Search sub files according to the specified path .
	* @Directory	Path, file path,such as ../../xxx/xxx1/ .
	* @File         Get the found file .
	* @bRecursively Recursive search .
	* return		If it's true, it's found .
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "FF"), Category = "SimpleHotUpdate")
	static bool FindFile(const FString& Directory, TArray<FString>& File, bool bRecursively = true);



	/*
	* Install a path that can be manually installed,It is usually based on maps or special props.
	* @PakFilename   According to the installation path of Pak, such as "../../yourpath/xxx.pak".
	* @PakOrder		 This is related to the Pak of the project itself
							1 ProjectSavedDir.
							2 EngineContentDir.
							3 ProjectContentDir.
							4 ../Content/Paks/ProjectName.
	* @MountPath	 Path of package installation  .
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "M"), Category = "SimpleHotUpdate")
	static bool MountPak(const FString& PakFilename, int32 PakOrder, const FString& MountPath,bool bSelfAdaptionMountProjectPath = false);

	/*
	* You can manually uninstall the path.
	* @PakFilename	Pak path ,such as "../../yourpath/xxx.pak".
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "UM"), Category = "SimpleHotUpdate")
	static bool UnmountPak(const FString& PakFilename);

	/*
	* Getting objects from Pak ,If the Pak is already Mounted .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "OB"), Category = "SimpleHotUpdate")
	static UObject* GetObjectFromPak(const FString& Filename);

	/*
	* Get the class from the Pak file  ,If the Pak is already Mounted .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "GC"), Category = "SimpleHotUpdate")
	static UClass* GetClassFromPak(const FString& Filename, const FString& InSuffix = TEXT(""), const FString& InPrefix = TEXT(""));
	
	/*
	* Getting Material from Pak  ,If the Pak is already Mounted .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	* @Type					Is the resource type patch or DLC ..
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "GM"), Category = "SimpleHotUpdate")
	static UMaterial* GetMaterialFromPak(const FString& Filename);

	/*
	* Getting Texture 2D from Pak  ,If the Pak is already Mounted .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	* @Type					Is the resource type patch or DLC ..
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "GT"), Category = "SimpleHotUpdate")
	static UTexture2D* GetTexture2DFromPak(const FString& Filename);
	
	/*
	* Getting Skeletal Mesh from Pak  ,If the Pak is already Mounted .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	* @Type					Is the resource type patch or DLC ..
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "GS"), Category = "SimpleHotUpdate")
	static USkeletalMesh* GetSkeletalMeshFromPak(const FString& Filename);
	
	/*
	* Getting static mesh from Pak  ,If the Pak is already Mounted .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	* @Type					Is the resource type patch or DLC ..
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "GSM"), Category = "SimpleHotUpdate")
	static UStaticMesh* GetStaticMeshFromPak(const FString& Filename);
	
	/*
	* Can be directly loaded into the Pak object and generated  ,If the Pak is already Mounted .
	* @WorldContextObject	If it is a blueprint, this parameter can be ignored. If it is C++,
							a world is needed because the current object needs to be generated in a world .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	* @InLocation			A character needs a place .
	* @InRotator			A character needs a rotation default .
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "OB", Category = "SimpleHotUpdate", WorldContext = "WorldContextObject"))
	static AActor* GetActorFromPak(UObject* WorldContextObject, const FString& Filename, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotator = FRotator::ZeroRotator);

	/*
	* Can be directly loaded into the Pak object and generated  ,If the Pak is already Mounted .
	* @WorldContextObject	If it is a blueprint, this parameter can be ignored. If it is C++,
							a world is needed because the current object needs to be generated in a world .
	* @Filename				The file format is similar to the following ："/Game/Mesh/YourAssetName" .
	* @InLocation			A character needs a place .
	* @InRotator			A character needs a rotation default .
	*/
	UFUNCTION(BlueprintPure, meta = (Keywords = "UMGWidget", Category = "SimpleHotUpdate", WorldContext = "WorldContextObject"))
	static UUserWidget* GetUMGFromPak(UObject* WorldContextObject,const FString& Filename);
};
