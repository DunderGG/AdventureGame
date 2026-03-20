// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "SunMoonController.h"
#include "MessagingSubsystem.h"
#include "Logger.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Curves/CurveLinearColor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASunMoonController::ASunMoonController()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASunMoonController::timeChangeUpdate(const FTimeData& timeData)
{
	updateTimeOfDay(timeData);
}

// This is called every tick instead of every in game minute, so that sun movement is less "blocky".
void ASunMoonController::preciseTimeChangeUpdate(float preciseTime)
{
	timeOfDayRef = preciseTime;

	updateSunLightPrecise();
	updateSkyLight();
	updateMoonLight();
}

/*
* This is called every in - game minute instead.
* currentTime is currently not used, as we are using the more precise timeOfDayRef instead.
* 
* TODO: Maybe just delete this function and move the contents to timeChangeUpdate() instead.
*/ 
void ASunMoonController::updateTimeOfDay(const FTimeData& newTime)
{
	currentTime = newTime;
	Logger::addMessage(FString::Printf(TEXT("SunMoonController: Time updated to %d:%d" 
											" on day %d of month %d, year %d"), 
		currentTime.hour, currentTime.minute, 
		currentTime.dayOfMonth, currentTime.month, currentTime.year), 
		SEVERITY::Info);
	Logger::addMessage(FString::Printf(TEXT("SunMoonController: Time of day ref is now %f"), timeOfDayRef), SEVERITY::Info);
}

/*
* This runs whenever there is a time change update broadcasted from the EnvironmentManager.
* Use the time of day to set where the sun is in the sky and how bright it is.
*/
/*
void ASunMoonController::updateSunLight()
{
	// We set the sunLight variable in the editor.
	if (!sunLight || !dailySunRotation)
	{
		Logger::addMessage(TEXT("SunMoonController: No sun light assigned, skipping sun light update"), 
			SEVERITY::Warning);
		return;
	}

	currentTimeOfDay = currentTime.getTimeOfDay();
	float newLightIntensity = dailySunRotation->GetUnadjustedLinearColorValue(currentTimeOfDay).A;
	FLinearColor colorAsRotation = dailySunRotation->GetUnadjustedLinearColorValue(currentTimeOfDay);

	if (annualSunRotation)
	{
		newLightIntensity += annualSunRotation->GetUnadjustedLinearColorValue(currentTimeOfDay).A;
		colorAsRotation += annualSunRotation->GetLinearColorValue(currentTimeOfDay);
	}
	newLightIntensity = FMath::Clamp(newLightIntensity, 0.0f, maxSunIntensity);

	FRotator newLightRotation = FRotator(colorAsRotation.G, colorAsRotation.B, colorAsRotation.R);

	sunComponent->SetIntensity(newLightIntensity);
	sunLight->SetActorRotation(newLightRotation);
	sunComponent->UpdateColorAndBrightness();
}
*/
/*
* The same as above, but happens every tick to try and make the movement of the sun more smooth.
*  TODO: There are probably better ways to do this, 
*			than broadcasting and doing this logic every tick.
*/
void ASunMoonController::updateSunLightPrecise()
{
	// Directly use the float time to calculate rotation
	if (!sunLight || !dailySunRotation)
	{
		Logger::addMessage(TEXT("SunMoonController: No sun light assigned, skipping sun light update"),
			SEVERITY::Warning);
		return;
	}

	float newLightIntensity = dailySunRotation->GetUnadjustedLinearColorValue(timeOfDayRef).A;
	FLinearColor colorAsRotation = dailySunRotation->GetUnadjustedLinearColorValue(timeOfDayRef);

	if (annualSunRotation)
	{
		newLightIntensity += annualSunRotation->GetUnadjustedLinearColorValue(timeOfDayRef).A;
		colorAsRotation += annualSunRotation->GetUnadjustedLinearColorValue(timeOfDayRef);
	}
	newLightIntensity = FMath::Clamp(newLightIntensity, 0.0f, maxSunIntensity);

	FRotator newLightRotation = FRotator(colorAsRotation.G, colorAsRotation.B, colorAsRotation.R);
	sunLight->SetActorRotation(newLightRotation);
	sunComponent->SetIntensity(newLightIntensity);
	sunComponent->UpdateColorAndBrightness();
	//Logger::addMessage(FString::Printf(TEXT("SunMoonController: Precise time update - sun light intensity set to %f and rotation to "), newLightIntensity) + newLightRotation.ToString(), SEVERITY::Debug);
}

void ASunMoonController::updateSkyLight()
{
	if (!IsValid(skyLight) || !IsValid(skyLightDailyColor))
	{
		//Logger::addMessage(TEXT("SunMoonController: No sky light assigned, skipping sky light update"),
		//	SEVERITY::Warning);
		return;
	}

	float newLightIntensity = skyLightDailyColor->GetUnadjustedLinearColorValue(timeOfDayRef).A;
	skyLightComponent->SetIntensity(newLightIntensity);

	FLinearColor newSkyColor = skyLightDailyColor->GetUnadjustedLinearColorValue(timeOfDayRef);
	skyLightComponent->SetLightColor(newSkyColor);
}

void ASunMoonController::updateMoonLight()
{
}

// Called when the game starts or when spawned
void ASunMoonController::BeginPlay()
{
	Super::BeginPlay();
	
	// Cache the components so we don't need to fetch them every tick.
	if (sunLight)
	{
		sunComponent = sunLight->GetLightComponent();
	}
	if (skyLight)
	{
		skyLightComponent = skyLight->GetLightComponent();
	}

	if (hasDayNightCycle)
	{
		Logger::addMessage(TEXT("SunMoonController: Day/Night cycle enabled"), SEVERITY::Info);
		TObjectPtr<UWorld> world = GetWorld();
		if (IsValid(world) && world->IsGameWorld())
		{
			// Cache the messaging subsystem from the world game instance.
			messageManager = UMessagingSubsystem::Get(this);

			if (IsValid(messageManager))
			{
				messageManager->onTimeChange.AddDynamic(this, &ASunMoonController::timeChangeUpdate);
				messageManager->onPreciseTimeChange.AddDynamic(this, &ASunMoonController::preciseTimeChangeUpdate);
				Logger::addMessage(TEXT("SunMoonController: Subscribed to time change delegate"), SEVERITY::Debug);
			}
			else
			{
				Logger::addMessage(TEXT("SunMoonController: Failed to get MessagingSubsystem"), SEVERITY::Error);
			}
		}
	}
	else
	{
		Logger::addMessage(TEXT("SunMoonController: Day/Night cycle disabled"), SEVERITY::Info);
		return;
	}
}

/*
* Clean up subscriptions to prevent any edge-case issues during level transitions.
*/
void ASunMoonController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (messageManager)
	{
		messageManager->onTimeChange.RemoveDynamic(this, &ASunMoonController::timeChangeUpdate);
		messageManager->onPreciseTimeChange.RemoveDynamic(this, &ASunMoonController::preciseTimeChangeUpdate);
	}
	Super::EndPlay(EndPlayReason);
}
