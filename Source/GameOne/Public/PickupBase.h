// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCharacter.h"
#include "PickupBase.generated.h"

class UItemDefinition;

// The BlueprintType and Blueprintable specifiers expose it as a base class for creating Blueprints
UCLASS(BlueprintType, Blueprintable)
class GAMEONE_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initializes this pickup with values from the data table
	void initializePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Pointer to the Data Table that the pickup uses to get its data
	// The Data Table may not be loaded at runtime, so use a TSoftObjectPtr here so you can load it asynchronously.
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	TSoftObjectPtr<UDataTable> pickupDataTable;

	// Pickups shouldn’t have a default item ID, so the EditInstanceOnly specifier 
	// enables you to edit this property in instances of pickups in the world, but not in the archetype(or class default).
	// The | in category creates a Pickup section, with an Item Table subsection 
	// 
	// The id of this pickup in the associated data table
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	FName pickupItemId;

	// Data asset associated with this item
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Reference Item")
	TObjectPtr<UItemDefinition> referenceItem;

	// The mesh component to represent the pickup in the world
	// It should be visible, but not editable, in Unreal Editor
	UPROPERTY(VisibleDefaultsOnly, Category = "Pickup | Mesh")
	TObjectPtr<USkeletalMeshComponent> pickupMeshComponent;

	// Sphere Component that defines the collision radius of this pickup for interaction purposes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Components")
	TObjectPtr<USphereComponent> sphereComponent;

	// Code for when something overlaps the SphereComponent
	/*
	* Any component that inherits from UPrimitiveComponent, like USphereComponent, can implement this function 
		to run code when the component overlaps with some other Actor. 
		This function takes several parameters you won’t be using; you’ll only pass the following:
			UPrimitiveComponent* OverlappedComponent: The component that was overlapped.
			AActor* OtherActor: The Actor overlapping that component.
			UPrimitiveComponent* OtherComp: The Actor’s component that overlapped.
			int32 OtherBodyIndex: The index of the overlapped component.
			bool bFromSweep, const FHitResult& SweepResult: Information about the collision, such as where it happened and at what angle.
	*/
	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* overlappedComponent, 
		AActor* otherActor, 
		UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& sweepResult);

	// Whether this pickup should respawn after being picked up
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Respawn")
	bool bShouldRespawn;

	// The time in seconds to wait before respawning
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Respawn")
	float respawnTime = 2.0f;

	// Timer handle to distinguish the respawn timer
	FTimerHandle respawnTimerHandle;

// The WITH_EDITOR macro tells Unreal Header Tool that anything inside should only be packaged for editor builds
// and not compiled for release versions of the game
#if WITH_EDITOR
	// Runs whenever a property on this object is changed in the editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif
};
