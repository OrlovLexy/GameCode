#include "SaveSubsystemTypes.h"

DEFINE_LOG_CATEGORY(LogSaveSubsystem);

FSaveSubsystemArchive::FSaveSubsystemArchive(FArchive& InInnerArchive, bool bInLoadIfFindFails)
	: FObjectAndNameAsStringProxyArchive(InInnerArchive, bInLoadIfFindFails)
{
	ArIsSaveGame = true;
	ArNoDelta = true;
}

FSaveDirectoryVisitor::FSaveDirectoryVisitor(TArray<int32>& InSaveIds)
	: SaveIds(InSaveIds)
{
}

bool FSaveDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (bIsDirectory)
	{
		return true;
	}

	const FString FullFilePath(FilenameOrDirectory);

	FString DirectoryName;
	FString Filename;
	FString FileExtension;
	FPaths::Split(FullFilePath, DirectoryName, Filename, FileExtension);
	if (FileExtension == FileExtensionSave.ToString())
	{
		const int32 SaveId = FCString::Atoi(*Filename);
		if (SaveId > 0)
		{
			SaveIds.AddUnique(SaveId);
		}
	}

	return true;
}

BoolScopeWrapper::BoolScopeWrapper(bool& bInValue, bool bNewValue)
	: bValue(bInValue)
	, bInitialValue(bInValue)
{
	bValue = bNewValue;
}

BoolScopeWrapper::~BoolScopeWrapper()
{
	bValue = bInitialValue;
}