#include "Logger.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY(DevLog);

FCriticalSection Logger::logMutex;

FString Logger::severityAsString(const SEVERITY& severity)
{
	switch (severity)
	{
	case SEVERITY::Debug:
		return TEXT("DEBUG");
	case SEVERITY::Info:
		return TEXT("INFO");
	case SEVERITY::Warning:
		return TEXT("WARNING");
	case SEVERITY::Error:
		return TEXT("***ERROR***");
	case SEVERITY::Critical:
		return TEXT("!!!CRITICAL!!!");
	default:	//Should never happen.
		return TEXT("UNKNOWN ERROR");
	}
}

FColor Logger::severityAsColor(const SEVERITY& severity)
{
	switch (severity)
	{
	case SEVERITY::Debug:
		return FColor::Green;
	case SEVERITY::Info:
		return FColor::Cyan;
	case SEVERITY::Warning:
		return FColor::Yellow;
	case SEVERITY::Error:
		return FColor::Red;
	case SEVERITY::Critical:
		return FColor::Magenta;
	default:
		return FColor::White;
	}
}
/*
* Can print to
*	1. Unreal Output Window (visible in Editor)
*	2. Log File (<Project>/Saved/Logs/DevLog_Date.txt)
*	3. On-Screen Debug Message (In Editor PIE, only if on the Game Thread)
*/
void Logger::addMessage(const FString& message, 
						const SEVERITY severity,
						const float onScreenDuration,
						const bool toUeLog,
						const bool toFile,
						const bool toScreen)
{
	const FDateTime dateTime = FDateTime::Now();
	const FString date = dateTime.ToString(TEXT("%Y-%m-%d"));

	const FString path = FPaths::ProjectLogDir() + TEXT("DevLog_") + date + TEXT(".txt");

	const FString severityStr = severityAsString(severity);
	const FString dateStr = dateTime.ToString(TEXT("%Y-%m-%d"));
	const FString timeStr = dateTime.ToString(TEXT("%H:%M:%S"));
	const FString logMessage = FString::Printf(TEXT("[%s] [%s] [%s]:  %s"), *severityStr, *dateStr, *timeStr, *message);

	// 1. Log to Unreal Output Window
	// This makes it visible in the Editor and standard Engine logs
	if (toUeLog)
	{
		UE_LOG(DevLog, Log, TEXT("%s"), *logMessage);
	}

	// 2. Log to File (Standard Unreal Way), lock the mutex to prevent concurrent writes from multiple threads
	if (toFile)
	{
		FScopeLock Lock(&logMutex);

		FFileHelper::SaveStringToFile(logMessage + LINE_TERMINATOR, 
									  *path, 
									  FFileHelper::EEncodingOptions::AutoDetect, 
									  &IFileManager::Get(), 
									  FILEWRITE_Append);
	}

	// 3. Log to Screen (In Editor), (only if on the Game Thread)
#if UE_EDITOR
	if (toScreen && GEngine && IsInGameThread())
	{
		GEngine->AddOnScreenDebugMessage(-1, onScreenDuration, severityAsColor(severity), logMessage);
	}
#endif
}
