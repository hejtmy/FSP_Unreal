// Fill out your copyright notice in the Description page of Project Settings.

#include "Logging/FSPLog.h"

UFSPLog::UFSPLog()
{
	TodayUnixTimestamp = 0;
}

bool UFSPLog::CreateFile(const FString Text)
{
	const FString ID{ "NEO" };
	
	TodayUnixTimestamp = FDateTime::Today().ToUnixTimestamp();
	Log = new FSPLogWriter(ID, Text);
	return true;
}

float UFSPLog::GetTimestamp() const
{
	const FDateTime Now{ FDateTime::Now() };
	float TimestampNow = static_cast<float>(Now.ToUnixTimestamp() - TodayUnixTimestamp);
	const FString ms = FString::FromInt(Now.GetMillisecond());
	TimestampNow += Now.GetMillisecond() / 1000.0f;
	return TimestampNow;
}

FString UFSPLog::CreateTimestamp() const
{
	return FString::Printf(TEXT("%5.4f"), this->GetTimestamp());
}

bool UFSPLog::WriteLine(const FString Text, bool AllowOverwrite) const
{
	Log->WriteLine(Text);
	return true;
}

bool UFSPLog::WriteArray(TArray<FString> Text, bool AllowOverwrite, FString Delim, bool AddTimestamp) const
{
	TArray<FString> Arr;
	if(AddTimestamp) Arr.Add(CreateTimestamp());
	Arr.Append(Text);
	Log->WriteArray(Arr, Delim);
	return true;
}

/**
 * Writes given line and prepends timestamp
 *
 *@param Text text to be written into given file
 */
bool UFSPLog::WriteMessage(const FString Text) const
{
	return WriteArray(TArray<FString>{Text});
}

void UFSPLog::CloseFile()
{
	Log->Close();
	delete Log;
}

bool UFSPLog::IsLogOpen() const
{
	if(Log == nullptr) return false;
	return Log->IsOpen();
}