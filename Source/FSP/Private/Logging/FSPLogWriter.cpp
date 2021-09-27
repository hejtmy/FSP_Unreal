// Fill out your copyright notice in the Description page of Project Settings.
#include "Logging/FSPLogWriter.h"

FSPLogWriter::FSPLogWriter(FString Prefix, FString LogName, FString Timestamp)
{
	FString LogTimestamp;
	if(Timestamp == "")
	{
		const FDateTime CurrentDateTime = FDateTime::Now();
		LogTimestamp = CurrentDateTime.ToString();
	}
	else
	{
		LogTimestamp = Timestamp;
	}
	FilePath = CreateFilepath(&Prefix, &LogName, &LogTimestamp);
	this->Open();
}

FSPLogWriter::~FSPLogWriter()
{
	
}

bool FSPLogWriter::Open()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString Directory = FPaths::GetPath(FilePath);
	if (PlatformFile.CreateDirectoryTree(*Directory))
	{
		// Check if file exists and can be written to.
		//if(!FPaths::FileExists(FilePath))
		return true;
	}
	return false;
}

void FSPLogWriter::Close()
{
	// filehandle is never created
	//delete File;
	FilePath.Reset();
}

FString FSPLogWriter::CreateFilepath(FString* Prefix, FString* LogName, FString* Timestamp) const
{
	FString Filepath = FPaths::ProjectDir();
	Filepath += RelativePath + FGenericPlatformMisc::GetDefaultPathSeparator();
	Filepath += *Prefix + TEXT("_") + *LogName + TEXT("_") + *Timestamp + TEXT(".txt");
	return Filepath;
}

bool FSPLogWriter::IsOpen() const
{
	return !FilePath.IsEmpty();
}

void FSPLogWriter::WriteLine(FString Line) const
{
	if (!IsOpen()) {
		UE_LOG(LogTemp, Warning, TEXT("The log is not open"));
		return;
	}
	FString Txt = Line;
	Txt += LINE_TERMINATOR;
	FFileHelper::SaveStringToFile(Txt, *FilePath,
		FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(),
		FILEWRITE_Append);
	
}

void FSPLogWriter::WriteArray(TArray<FString> Arr, FString Delim) const
{
	if (!IsOpen()) return;
	const TCHAR* Delimiter = *Delim;
	const FString Out = FString::Join(Arr, Delimiter);
	WriteLine(Out);
}
