

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MessageManager.generated.h"


/*
* A delegate is kind of like a mailing list,
* where you can subscribe to it and then when the event happens,
* all the subscribers will be notified.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHourChangeDelegate, int, hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDayOfYearChangeDelegate, int, dayOfYear);

UCLASS()
class MESSAGEHANDLER_API AMessageManager : public AActor
{
	GENERATED_BODY()

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AMessageManager();

#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	FHourChangeDelegate onHourChange;

	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Date")
	FDayOfYearChangeDelegate onDayOfYearChange;
#pragma endregion

#pragma region TRIGGERS
	void updateHourOfDay(int newHour);
	void updateDayOfYear(int newDayOfYear);
#pragma endregion
};
