// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

// project header
#include "FPatchVersionDiff.h"
#include "FChunkInfo.h"
#include "FReplaceText.h"
#include "ETargetPlatform.h"
#include "FExternAssetFileInfo.h"
#include "FExternDirectoryInfo.h"
#include "FPlatformExternAssets.h"
#include "FPatcherSpecifyAsset.h"
#include "FlibPatchParserHelper.h"
#include "HotPatcherSettingBase.h"

// engine header
#include "CoreMinimal.h"
#include "FPlatformExternAssets.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "FExportPatchSettings.generated.h"


/** Singleton wrapper to allow for using the setting structure in SSettingsView */
USTRUCT(BlueprintType)
struct FExportPatchSettings:public FHotPatcherSettingBase
{
	GENERATED_USTRUCT_BODY()
public:

	FExportPatchSettings();

	FORCEINLINE static FExportPatchSettings* Get()
	{
		static FExportPatchSettings StaticIns;

		return &StaticIns;
	}

	FORCEINLINE TArray<FExternAssetFileInfo> GetAllExternFiles(bool InGeneratedHash=false)const
	{
		TArray<FExternAssetFileInfo> AllExternFiles = UFlibPatchParserHelper::ParserExDirectoryAsExFiles(GetAddExternDirectory());

		for (auto& ExFile : GetAddExternFiles())
		{
			if (!AllExternFiles.Contains(ExFile))
			{
				AllExternFiles.Add(ExFile);
			}
		}
		if (InGeneratedHash)
		{
			for (auto& ExFile : AllExternFiles)
			{
				ExFile.GenerateFileHash();
			}
		}
		return AllExternFiles;
	}

	FORCEINLINE TArray<FDirectoryPath> GetAssetIncludeFilters()const { return AssetIncludeFilters; }
	TArray<FString> GetAssetIgnoreFiltersPaths()const;
	FORCEINLINE TArray<FDirectoryPath> GetAssetIgnoreFilters()const { return AssetIgnoreFilters; }
	FORCEINLINE bool IsAnalysisFilterDependencies()const { return bAnalysisFilterDependencies; }
	FORCEINLINE TArray<EAssetRegistryDependencyTypeEx> GetAssetRegistryDependencyTypes()const { return AssetRegistryDependencyTypes; }
	FORCEINLINE TArray<FPatcherSpecifyAsset> GetIncludeSpecifyAssets()const { return IncludeSpecifyAssets; }

	// pak command
	TArray<FString> MakeAddExternFileToPakCommands()const;
	// TArray<FString> GetAllExternalCookCommands()const;
	TArray<FString> MakeAllExternDirectoryAsPakCommand()const;
	TArray<FString> MakeAllPakCommandsByTheSetting(const FString& InPlatformName, const FPatchVersionDiff& InVersionDiff, bool bDiffExFiles = true)const;
	bool MakeAllExternAssetAsPakCommands(const FString& InProjectDir, const FString& InPlatform, const TArray<FString>& PakOptions, TArray<FString>& OutPakCommands)const;

	FString GetSaveAbsPath()const;
	bool SerializePatchConfigToJsonObject(TSharedPtr<FJsonObject>& OutJsonObject)const;
	bool SerializePatchConfigToString(FString& OutSerializedStr)const;
	void DeserializePatchConfig(const FString& InContent);

	FORCEINLINE FString GetVersionId()const { return VersionId; }
	FORCEINLINE FString GetBaseVersion()const { return BaseVersion.FilePath; }
	FORCEINLINE TArray<FString> GetPakCommandOptions()const { return PakCommandOptions; }
	FORCEINLINE TArray<FReplaceText> GetReplacePakCommandTexts()const { return ReplacePakCommandTexts; }
	FORCEINLINE TArray<FString> GetUnrealPakOptions()const { return UnrealPakOptions; }
	FORCEINLINE TArray<ETargetPlatform> GetPakTargetPlatforms()const { return PakTargetPlatforms; }
	TArray<FString> GetPakTargetPlatformNames()const;

	FORCEINLINE bool IsSavePakList()const { return bSavePakList; }
	FORCEINLINE bool IsSaveDiffAnalysis()const { return IsByBaseVersion() && bSaveDiffAnalysis; }
//	FORCEINLINE bool IsSavePakVersion()const { return bSavePakVersion; }
	FORCEINLINE bool IsSavePatchConfig()const { return bSavePatchConfig; }

	FORCEINLINE bool IsIncludeAssetRegistry()const { return bIncludeAssetRegistry; }
	FORCEINLINE bool IsIncludeGlobalShaderCache()const { return bIncludeGlobalShaderCache; }
	FORCEINLINE bool IsIncludeShaderBytecode()const { return bIncludeShaderBytecode; }
	FORCEINLINE bool IsIncludeEngineIni()const { return bIncludeEngineIni; }
	FORCEINLINE bool IsIncludePluginIni()const { return bIncludePluginIni; }
	FORCEINLINE bool IsIncludeProjectIni()const { return bIncludeProjectIni; }

	FORCEINLINE bool IsByBaseVersion()const { return bByBaseVersion; }
	FORCEINLINE bool IsEnableExternFilesDiff()const { return bEnableExternFilesDiff; }
	FORCEINLINE bool IsIncludeHasRefAssetsOnly()const { return bIncludeHasRefAssetsOnly; }
	FORCEINLINE bool IsIncludePakVersion()const { return bIncludePakVersionFile; }
	FORCEINLINE bool IsSaveAssetRelatedInfo()const { return bSaveAssetRelatedInfo; }

	// chunk infomation
	FORCEINLINE bool IsEnableChunk()const { return bEnableChunk; }
	FORCEINLINE TArray<FChunkInfo> GetChunkInfos()const { return ChunkInfos; }

	FORCEINLINE FString GetPakVersionFileMountPoint()const { return PakVersionFileMountPoint; }
	FORCEINLINE TArray<FExternAssetFileInfo> GetAddExternFiles()const { return AddExternFileToPak; }
	FORCEINLINE TArray<FExternDirectoryInfo> GetAddExternDirectory()const { return AddExternDirectoryToPak; }
	static FPakVersion GetPakVersion(const FHotPatcherVersion& InHotPatcherVersion,const FString& InUtcTime);
	static FString GetSavePakVersionPath(const FString& InSaveAbsPath,const FHotPatcherVersion& InVersion);
	static FString GetPakCommandsSaveToPath(const FString& InSaveAbsPath, const FString& InPlatfornName, const FHotPatcherVersion& InVersion);

	FHotPatcherVersion GetNewPatchVersionInfo()const;
	bool GetBaseVersionInfo(FHotPatcherVersion& OutBaseVersion)const;
	FString GetCurrentVersionSavePath()const;

	FORCEINLINE bool IsCustomPakNameRegular()const {return bCustomPakNameRegular;}
	FORCEINLINE FString GetPakNameRegular()const { return PakNameRegular;}

	FORCEINLINE TArray<FString> GetAssetIncludeFiltersPaths()const
	{
		TArray<FString> Result;
		for (const auto& Filter : AssetIncludeFilters)
		{
			if (!Filter.Path.IsEmpty())
			{
				Result.AddUnique(Filter.Path);
			}
		}
		return Result;
	}
	
	FORCEINLINE TArray<FExternAssetFileInfo> GetAllExternFilesByPlatform(ETargetPlatform InTargetPlatform,bool InGeneratedHash = false)const
	{
		TArray<FExternAssetFileInfo> AllExternFiles = UFlibPatchParserHelper::ParserExDirectoryAsExFiles(GetAddExternDirectoryByPlatform(InTargetPlatform));
	
		for (auto& ExFile : GetAddExternFilesByPlatform(InTargetPlatform))
		{
			if (!AllExternFiles.Contains(ExFile))
			{
				AllExternFiles.Add(ExFile);
			}
		}
		if (InGeneratedHash)
		{
			for (auto& ExFile : AllExternFiles)
			{
				ExFile.GenerateFileHash();
			}
		}
		return AllExternFiles;
	}
	
	FORCEINLINE TMap<ETargetPlatform,FHotPatcherPlatformFiles> GetAllPlatfotmExternFiles(bool InGeneratedHash = false)const
	{
		TMap<ETargetPlatform,FHotPatcherPlatformFiles> result;
	
		for(const auto& Platform:GetAddExternAssetsToPlatform())
		{
			FHotPatcherPlatformFiles PlatformIns{Platform.TargetPlatform,GetAllExternFilesByPlatform(Platform.TargetPlatform,InGeneratedHash)};
			result.Add(Platform.TargetPlatform,PlatformIns);
		}
		return result;
	}

	FORCEINLINE TArray<FPlatformExternAssets> GetAddExternAssetsToPlatform()const{return AddExternAssetsToPlatform;}
	
	FORCEINLINE TArray<FExternAssetFileInfo> GetAddExternFilesByPlatform(ETargetPlatform InTargetPlatform)const
	{
		for(const auto& Platform:GetAddExternAssetsToPlatform())
		{
			if (Platform.TargetPlatform == InTargetPlatform)
			{
				return Platform.AddExternFileToPak;
			}
		}

		return TArray<FExternAssetFileInfo>{};
	}
	FORCEINLINE TArray<FExternDirectoryInfo> GetAddExternDirectoryByPlatform(ETargetPlatform InTargetPlatform)const
	{
		for(const auto& Platform:GetAddExternAssetsToPlatform())
		{
			if (Platform.TargetPlatform == InTargetPlatform)
			{
				return Platform.AddExternDirectoryToPak;
			}
		}

		return TArray<FExternDirectoryInfo>{};
	}

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseVersion")
		bool bByBaseVersion = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "BaseVersion",meta = (RelativeToGameContentDir, EditCondition="bByBaseVersion"))
		FFilePath BaseVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PatchBaseSettings")
		FString VersionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Asset Filter",meta = (RelativeToGameContentDir, LongPackageName))
		TArray<FDirectoryPath> AssetIncludeFilters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Filter", meta = (RelativeToGameContentDir, LongPackageName))
		TArray<FDirectoryPath> AssetIgnoreFilters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Filter")
		bool bIncludeHasRefAssetsOnly;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Filter")
		bool bAnalysisFilterDependencies=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Filter",meta = (EditCondition="bAnalysisFilterDependencies"))
		TArray<EAssetRegistryDependencyTypeEx> AssetRegistryDependencyTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Specify Assets")
		TArray<FPatcherSpecifyAsset> IncludeSpecifyAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooked Files")
		bool bIncludeAssetRegistry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooked Files")
		bool bIncludeGlobalShaderCache;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooked Files")
		bool bIncludeShaderBytecode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ini Config Files")
		bool bIncludeEngineIni;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ini Config Files")
		bool bIncludePluginIni;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ini Config Files")
		bool bIncludeProjectIni;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extern Files")
		bool bEnableExternFilesDiff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extern Files")
		TArray<FExternAssetFileInfo> AddExternFileToPak;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extern Files")
		TArray<FExternDirectoryInfo> AddExternDirectoryToPak;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extern Files")
		TArray<FPlatformExternAssets> AddExternAssetsToPlatform;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extern Files")
		bool bIncludePakVersionFile = false;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extern Files",meta=(EditCondition = "bIncludePakVersionFile"))
		FString PakVersionFileMountPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Options")
		bool bEnableChunk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Options", meta = (EditCondition = "bEnableChunk"))
		TArray<FChunkInfo> ChunkInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pak Options")
		TArray<FString> PakCommandOptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pak Options")
		TArray<FReplaceText> ReplacePakCommandTexts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pak Options")
		TArray<FString> UnrealPakOptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pak Options")
		TArray<ETargetPlatform> PakTargetPlatforms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pak Options")
		bool bCustomPakNameRegular;
	// Can use value: {VERSION} {BASEVERSION} {CHUNKNAME} {PLATFORM} 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pak Options",meta=(EditCondition = "bCustomPakNameRegular"))
		FString PakNameRegular = TEXT("{VERSION}_{CHUNKNAME}_{PLATFORM}_001_P");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveTo")
		bool bSavePakList = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveTo",meta=(EditCondition="bByBaseVersion"))
		bool bSaveDiffAnalysis = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveTo")
		bool bSaveAssetRelatedInfo;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveTo", meta = (EditCondition = "bIncludePakVersion"))
	//	bool bSavePakVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveTo")
		bool bSavePatchConfig = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveTo")
		FDirectoryPath SavePath;

};