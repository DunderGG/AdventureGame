// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "PickupBase.generated.h"

class UItemDefinition;

UCLASS(BlueprintType, Blueprintable)
class PICKUPS_API APickupBase : public AActor
{
	GENERATED_BODY()
	
private:	

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void onSphereBeginOverlap(UPrimitiveComponent* overlappedComponent, 
							  AActor* otherActor, 
							  UPrimitiveComponent* otherComp, 
							  int32 otherBodyIndex, 
							  bool fromSweep, 
							  const FHitResult& sweepResult);

	// The Id of this pickup in the associated data table.
	// Pickups shouldn’t have a default item ID, 
	// so the EditInstanceOnly specifier lets you edit this property in instances of pickups in the world, 
	// but not in the archetype (or class default).
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	FName pickupItemId;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	TSoftObjectPtr<UDataTable> pickupDataTable;

	// Data Asset associated with this item.
	// By saving the data in a reference item like this,
	// Unreal Engine can easily reference that data when it needs to know about the item 
	// instead of performing more table data lookups, which is less efficient.
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Reference Item")
	TObjectPtr<UItemDefinition> referenceItem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Pickup | Mesh")
	TObjectPtr<UStaticMeshComponent> pickupMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Collision")
	TObjectPtr<USphereComponent> collisionSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Respawn")
	bool shouldRespawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Respawn")
	float respawnTime = 5.0f;

	FTimerHandle respawnTimerHandle;

public:	
	APickupBase();
	virtual void Tick(float DeltaTime) override;

	void initializePickup();
};
