// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/FileHelper.h"

/**
 * 
 */
class FSP_API FSPLogWriter
{
public:
	FSPLogWriter(FString Prefix, FString LogName, FString Timestamp = "");
	~FSPLogWriter();

	const FString RelativePath = "logs";
	FString FilePath;
	FString FileDirectory;
	
	bool Open() const;
	void Close() const;
	bool IsOpen() const;

	void WriteLine(FString line) const;
	void WriteArray(TArray<FString> Arr, FString Delim = TEXT(";")) const;

private:
	IFileHandle* File;
	FString CreateFilepath(FString* ParticipantId, FString* LogName, FString* Timestamp) const;
};
