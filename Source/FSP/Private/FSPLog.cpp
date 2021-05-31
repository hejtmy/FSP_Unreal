// Fill out your copyright notice in the Description page of Project Settings.

#include "FSPLog.h"

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
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ms);
	TimestampNow += Now.GetMillisecond() / 1000.0f;
	return TimestampNow;
}

bool UFSPLog::WriteLine(const FString Text, bool AllowOverwrite) const
{
	Log->WriteLine(Text);
	return true;
}

bool UFSPLog::WriteMessage(const FString Text) const
{
	const FString Timestamp = FString::Printf(TEXT("%5.4f"), this->GetTimestamp());
	TArray<FString> Arr;
	Arr.Add(Timestamp);
	Arr.Add(Text);
	Log->WriteArray(Arr);
	return true;
}

bool UFSPLog::IsLogOpen() const
{
	return Log->IsOpen();
}
