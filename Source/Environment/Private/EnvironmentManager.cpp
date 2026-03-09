// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "Logger.h"
#include "EnvironmentManager.h"

void UEnvironmentManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TObjectPtr<UWorld> world = GetWorld();

	// TODO: Not sure about IsGameWorld().
	if (IsValid(world) && world->IsGameWorld())
	{
		FString mapName = world->GetMapName();
		mapName = world->RemovePIEPrefix(mapName);

		// TODO: Remember to use this name for our main menu map.
		if (mapName.Equals("MainMenu", ESearchCase::IgnoreCase))
		{
			shouldTick = false;
			Logger::addMessage(TEXT("Environment Manager: Ticking disabled for MainMenu"), SEVERITY::Debug);
		}

		SetTickableTickType(ETickableTickType::Conditional);
	}

	Logger::addMessage(TEXT("Environment Manager Initialized"), SEVERITY::Debug);
}

void UEnvironmentManager::Deinitialize()
{
	Super::Deinitialize();
	Logger::addMessage(TEXT("Environment Manager Deinitialized"), SEVERITY::Debug);
}

bool UEnvironmentManager::IsTickable() const
{
	// Only tick instances created for a world, not the class template itself.
	return !IsTemplate() && shouldTick;
}

void UEnvironmentManager::Tick(float DeltaTime)
{
}

TStatId UEnvironmentManager::GetStatId() const
{
	// This identifies the class in the 'stat cycle' profiler (F9 in-game)
	// TODO: Not sure how this works, investigate?
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEnvironmentManager, STATGROUP_Tickables);
}
