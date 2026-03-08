#pragma once
#include "CoreMinimal.h"

// Define a log category for this logger
DECLARE_LOG_CATEGORY_EXTERN(DevLog, Log, All);

enum class SEVERITY : uint8
{
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

/*
* Potential problems with a "global logger":
* Logging is often called from background threads (e.g., asynchronous loading, networking, or physics tasks). 
*	1.	File Access Violations: SaveStringToFile with FILEWRITE_Append is not thread-safe. 
*		If two threads call addMessage simultaneously, one will likely fail to open the file because it's locked by the other, 
*		resulting in lost logs.
*	2.	Engine Crashes: GEngine->AddOnScreenDebugMessage is not thread-safe and must be called from the Game Thread. 
*		Calling it from a worker thread will cause unpredictable crashes.
*/
class UTILITYFEATURES_API Logger
{
private:
	//Find the path to the log file.
	static FString severityAsString(const SEVERITY& severity);
	static FColor severityAsColor(const SEVERITY& severity);

	static FCriticalSection logMutex;

protected:


public:
	static void addMessage(const FString& message, 
						   const SEVERITY severity = SEVERITY::Info,
						   const float onScreenDuration = 5.0f,
						   const bool toUeLog = true, 
						   const bool toFile = true, 
						   const bool toScreen = true);

};
