


#include "MessageManager.h"


// Sets default values
AMessageManager::AMessageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

/*
* These are basically passthrough functions
*	When the time changes, we want to trigger the delegate, which will notify all subscribers that the time has changed.
*/
void AMessageManager::updateHourOfDay(int newHour)
{
	onHourChange.Broadcast(newHour);
}
void AMessageManager::updateDayOfYear(int newDayOfYear)
{
	onDayOfYearChange.Broadcast(newDayOfYear);
}

// Called when the game starts or when spawned
void AMessageManager::BeginPlay()
{
	Super::BeginPlay();

}



