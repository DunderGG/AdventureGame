// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "PickupBase.h"
#include "ItemDefinition.h"
#include "FItemData.h"
#include "Logger.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	pickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pickupMeshComponent"));
	check(pickupMeshComponent != nullptr);

	// Make the mesh the root component
	// If you don’t set a root component in C++, 
	// Unreal Engine may choose a different one during recompiles or editor restarts, 
	// which can break component attachment, transforms, and collision behavior.
	SetRootComponent(pickupMeshComponent);

	collisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphereComponent"));
	check(collisionSphereComponent != nullptr);
	collisionSphereComponent->SetupAttachment(pickupMeshComponent);
	//TODO radius needs experimentation, and different pickups need different values.
	collisionSphereComponent->SetSphereRadius(30.0f);
	collisionSphereComponent->SetGenerateOverlapEvents(true);

	// We want the character to be able to collide and trigger collision queries with the pickup, 
	// but the pickup shouldn’t have any physics that makes it bounce away when hit, 
	// so pass the ECollisionEnabled::QueryOnly option.
	collisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// We also need to define what types of collisions can occur. 
	// Collision channels define what types of objects can interact with each other and
	// collision responses define how those interactions are handled. 
	// Set the sphere's response to every collision channel to Ignore, removing any engine defaults
	collisionSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Then, set the response to the Pawn channel to Overlap, 
	// allowing the player character to trigger overlap events when they come into contact with the pickup.
	collisionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	// Ensure that we don't have any duplicate bindings to the overlap event,
	//   and then add our onSphereBeginOverlap function as a dynamic delegate.
	collisionSphereComponent->OnComponentBeginOverlap.RemoveAll(this);
	collisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::onSphereBeginOverlap);
	
	initializePickup();
}

/**
*	Broadcasts an event when a character overlaps this pickup's SphereComponent. 
*   Sets the pickup to invisible and uninteractable, and respawns it after a set time.
*
*	@param overlappedComponent - the component that was overlapped.
*	@param otherActor - the Actor overlapping this component.
*	@param otherComp - the Actor's component that overlapped this component.
*	@param otherBodyIndex - the index of the overlapped component.
*	@param fromSweep - whether the overlap was generated from a sweep.
*	@param sweepResult - contains info about the overlap such as surface normals and faces.
*/
void APickupBase::onSphereBeginOverlap(UPrimitiveComponent* overlappedComponent, 
									   AActor* otherActor, 
									   UPrimitiveComponent* otherComp, 
									   int32 otherBodyIndex, 
									   bool fromSweep, 
									   const FHitResult& sweepResult)
{
	Logger::addMessage("APickupBase::onSphereBeginOverlap - Overlap detected with Actor: " + otherActor->GetName());

	APlayerCharacter* character = Cast<APlayerCharacter>(otherActor);
	if (!character)
	{
		Logger::addMessage("APickupBase::onSphereBeginOverlap - Overlapping Actor is not a PlayerCharacter.", SEVERITY::Warning);
		return;
	}
	
	pickupMeshComponent->SetVisibility(false);
	pickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (shouldRespawn)
	{
		GetWorldTimerManager().SetTimer(respawnTimerHandle, 
										this, 
										&APickupBase::initializePickup, // TODO: This should be improved
										respawnTime, 
										false); // Repeat or no?
	}
	else
	{
		Destroy();
	}
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* To initialize a pickup, we need to retrieve a row of item data from the Data Table using the Pickup Item ID. 
* This lookup should only happen if the Id is set and a valid Data Table asset is assigned. 
* pickupDataTable is a soft reference and may not be loaded yet, so to validate the data table, 
*    you'll first convert that soft reference to an FSoftObjectPath
*
* An FSoftObjectPath stores an asset’s path without loading it, 
*    making it useful for validating soft references before using them.  
* 
* TODO: This should probably be refactored into two functions.
*          One for loading all the data,
*	       another that the timer can call to just respawn the item. (making it visible and collidable again)
*/
void APickupBase::initializePickup()
{
	const FSoftObjectPath dataTablePath(pickupDataTable.ToSoftObjectPath());

	if (pickupItemId.IsNone() || !dataTablePath.IsValid())
	{
		Logger::addMessage("PickupBase::initializePickup - Invalid Pickup Item ID or Data Table reference.");
		return;
	}

	UDataTable* dataTable = pickupDataTable.IsValid() 
		? pickupDataTable.Get() 
		: pickupDataTable.LoadSynchronous();

	if (!dataTable)
	{
		Logger::addMessage("PickupBase::initializePickup - Failed to load Data Table.");
		return;
	}

	// The ContextString is used for logging purposes to identify where the lookup is happening.
	const FItemData* itemDataRow = dataTable->FindRow<FItemData>(pickupItemId, pickupItemId.ToString());

	if (!itemDataRow)
	{
		Logger::addMessage("PickupBase::initializePickup - No item data row found for Pickup Item ID: " + pickupItemId.ToString());
		return;
	}

	// Get the item definition from the table row and make sure it's valid.
	UItemDefinition* itemDefinition = itemDataRow->itemDefinition.IsValid()
		? itemDataRow->itemDefinition.Get()
		: itemDataRow->itemDefinition.LoadSynchronous();

	if (!itemDefinition)
	{
		Logger::addMessage("PickupBase::initializePickup - Failed to load Item Definition for Pickup Item ID: " + pickupItemId.ToString());
		return;
	}

	// Build a runtime ItemDefinition object from the Data Table row.
	// From Epic:
	// In Unreal Engine, NewObject<T>() is a templated function for 
	// dynamically creating UObject-derived instances at runtime. 
	// It returns a pointer to the new object. It usually has the following syntax:
	//    T* Object = NewObject<T>(Outer, Class);
	// Where T is the type of UObject you're creating, 
	//       Outer is who owns this object, and 
	//       Class is the class of the object you're creating.
	// The Class argument is often T::StaticClass(), which gives a UClass pointer representing T's class type. 
	// However, you can often omit both arguments as UE assumes Outer is the current class and uses T to infer the UClass.
	referenceItem = NewObject<UItemDefinition>(this);

	referenceItem->id = itemDataRow->id;
	referenceItem->type = itemDataRow->type;
	referenceItem->name = itemDataRow->name;
	referenceItem->description = itemDataRow->description;
	referenceItem->mesh = itemDefinition->mesh;

	// Load the item mesh from the item definition.
	UStaticMesh* itemMesh = itemDefinition->mesh.IsValid() 
		? itemDefinition->mesh.Get() 
		: itemDefinition->mesh.LoadSynchronous();

	if (!itemMesh)
	{
		Logger::addMessage("PickupBase::initializePickup - Failed to load Item Mesh for Pickup Item ID: " + pickupItemId.ToString());
		return;
	}

	// Set the pickup mesh to the item mesh.
	pickupMeshComponent->SetStaticMesh(itemMesh);

	pickupMeshComponent->SetVisibility(true);
	pickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

