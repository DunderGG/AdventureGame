// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnvironmentManager.generated.h"

/**
 * UTickableWorldSubsystem combines UWorldSubsystem and FTickableGameObject.
 *    Must call it's respective Initialize and Deinitialize functions to make it tick.
 */
UCLASS()
class ENVIRONMENT_API UEnvironmentManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
private:
	bool shouldTick = true;
protected:

public:
	// UWorldSubsystem / FTickableGameObject overrides
	// There are also pre- and post-initialize functions.
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
