// Copyright dunder.gg. All Rights Reserved.

#include "PickupBase.h"
#include "Data/ItemDefinition.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create this pickup's mesh component and name it "PickupMesh"
	pickupMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("pickupMesh"));
	check(pickupMeshComponent != nullptr);

	// Create this pickup's sphere component
	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComponent"));
	check(sphereComponent != nullptr);

	// Attach the sphere component to the mesh component
	sphereComponent->SetupAttachment(pickupMeshComponent);

	// Set the sphere's collision radius
	sphereComponent->SetSphereRadius(32.0f);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	initializePickup();
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
*	Initializes the pickup with default values by retrieving them from the associated data table.
*	TODO Data Tables seem to work really poorly. For example, it seems like they refuse to load
*			unless we have the Data Table open in UE when running the game. 
*			Everyone seems to agree they are bad. Should use just Data Assets instead somehow?
*/
void APickupBase::initializePickup()
{
	// Only initialize if the pickup has valid inputs
	const FSoftObjectPath tablePath = pickupDataTable.ToSoftObjectPath();

	// Check that the data table exists and the item id has a value
	if (!tablePath.IsNull())
	{
		if (!pickupItemId.IsNone())
		{
			// Resolve the table soft reference into a usable data table.
			//	Use the table if available. Otherwise, load it now.
			//		I was hoping this would fix the issues with Data Tables, but nope...
			UDataTable* loadedDataTable = pickupDataTable.IsValid() ? pickupDataTable.Get() : pickupDataTable.LoadSynchronous();

			if (!loadedDataTable)
			{
				UE_LOG(LogTemp, Error, TEXT("APickupBase::initializePickup(): Failed to resolve a Data Table"));
				return;
			}
			/*
			* FindRow() takes two arguments:
				An FName row name you want to find. Pass the PickupItemID as the row name.
				An FString type context string that you can use for debugging if the row isn’t found.
				You can Text(“My context here.”) to add a context string, or use ToString() to convert the item ID into a context string.
			*/
			const FItemData* itemDataRow = loadedDataTable->FindRow<FItemData>(pickupItemId, pickupItemId.ToString());
			if (!itemDataRow)
			{
				UE_LOG(LogTemp, Error, TEXT("APickupBase::initializePickup(): Failed to find row '%s'"), *pickupItemId.ToString());
				return;
			}

			// Resolve the Data reference by this table row. If not available, load it now.
			UItemDefinition* tempItemDefinition = itemDataRow->itemBase.IsValid()
				? itemDataRow->itemBase.Get()
				: itemDataRow->itemBase.LoadSynchronous();
			if (!tempItemDefinition)
			{
				UE_LOG(LogTemp, Error, TEXT("APickupBase::%hs():%d: Failed getting Item Definition for row '%s'"), __func__, __LINE__, *pickupItemId.ToString());
				return;
			}

			// Create a copy of the item with the class type
			//  TODO It's never explained why we make this referenceItem, needs some investigation.
			referenceItem = tempItemDefinition->createItemCopy();
			
			// Before applying the WorldMesh to the pickup’s mesh component, 
			// we need to check that the mesh is loaded since we defined it with a TSoftObjectPtr.
			if (referenceItem->worldMesh.IsValid())
			{
				pickupMeshComponent->SetSkeletalMesh(referenceItem->worldMesh.Get());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("APickupBase::initializePickup(): Loading mesh synchronously..."));
				// If the mesh isn't loaded, load it by calling LoadSynchronous().
				USkeletalMesh* worldMesh = referenceItem->worldMesh.LoadSynchronous();
				pickupMeshComponent->SetSkeletalMesh(worldMesh);
			}

			// Set the mesh to be visible and collidable
			pickupMeshComponent->SetVisibility(true);
			sphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			// Register the Overlap Event
			//	OnSphereBeginOverlap() runs when a character collides with the pickup’s sphere component
			sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereBeginOverlap);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("APickupBase::initializePickup(): Incorrect Pickup item id '%s'"), *pickupItemId.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APickupBase::initializePickup(): Pickup Data Table not loaded properly"));
	}
}

/**
*	Broadcasts an event when a character overlaps this pickup's SphereComponent. 
*	Sets the pickup to invisible and uninteractable, and respawns it after a set time.
* 
*	@param OverlappedComponent - the component that was overlapped.
*	@param OtherActor - the Actor overlapping this component.
*	@param OtherComp - the Actor's component that overlapped this component.
*	@param OtherBodyIndex - the index of the overlapped component.
*	@param bFromSweep - whether the overlap was generated from a sweep.
*	@param SweepResult - contains info about the overlap such as surface normals and faces.
*/
void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// Checking if it's the MainCharacter overlapping the item
	AMainCharacter* character = Cast<AMainCharacter>(OtherActor);

	if (character != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("APickupBase::OnSphereBeginOverlap(): Picking up item  '%s'"), *referenceItem->id.ToString());
		// Give item to character
		character->giveItem(referenceItem);

		// Unregister from the Overlap Event so that it isn’t repeatedly triggered
		sphereComponent->OnComponentBeginOverlap.RemoveAll(this);

		// Set this pickup to be invisible and disable its collision
		pickupMeshComponent->SetVisibility(false);
		pickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		sphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APickupBase::OnSphereBeginOverlap(): Failed casting AMainCharacter somehow... '%s'"), *referenceItem->id.ToString());
	}

	// This can be changed in the editor
	if (bShouldRespawn)
	{
		/*
		* SetTimer(InOutHandle, Object, InFuncName, InRate, bLoop, InFirstDelay);
			Where:
				* InOutHandle is the FTimerHandle that’s controlling the timer (your respawnTimerHandle).

				* Object is the UObject that owns the function you’re calling. Use this.

				* InFuncName is a pointer to the function you want to call (InitializePickup() in this case).

				* InRate is a float value specifying the time in seconds to wait before calling your function.

				* bLoop makes the timer either repeat every Time seconds (true) or only fire once (false).

				* InFirstDelay (optional) is an initial time delay before the first function call in a looping timer. 
										If not specified, UE uses InRate as the delay.
										IMPORTANT: This needs to be <0 or unspecified, otherwise pickup will instantly respawn.
		*/
		if (!GetWorld()->GetTimerManager().IsTimerActive(respawnTimerHandle))
		{
			//UE_LOG(LogTemp, Display, TEXT("APickupBase::OnSphereBeginOverlap(): Setting respawn timer for item: '%s'"), *referenceItem->id.ToString());
			GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this, &APickupBase::initializePickup, respawnTime, false, -1);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("APickupBase::OnSphereBeginOverlap(): Respawn timer already active for item: '%s'"), *referenceItem->id.ToString());
		}
	}
}

/**
*	Updates this pickup whenever a property is changed in the Unreal editor.
*	@param propertyChangedEvent - contains info about the property that was changed.
*/
void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	// First handle parent class property changes
	Super::PostEditChangeProperty(propertyChangedEvent);

	// If a property was changed, get its name. If not, use None.
	//	NAME_None is a global Unreal Engine constant of type FName that means "no valid name" or “null name”.
	const FName changedPropertyName = propertyChangedEvent.Property ? propertyChangedEvent.Property->GetFName() : NAME_None;

	// Verify that the Data Table is valid, and that the changed property exists in this class
	if (pickupDataTable)
	{					// This macro does a compile-time check to ensure the property exists in the passed class
		if (changedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, pickupItemId))
		{
			// Retrieve the associated ItemData for the pickup
			if (const FItemData* itemDataRow = pickupDataTable->FindRow<FItemData>(pickupItemId, pickupItemId.ToString()))
			{
				// Store the Data Asset found in Data Table
				UItemDefinition* tempItemDefinition = itemDataRow->itemBase.Get();

				// Update the pickup's mesh to the new one, after we changed the property
				pickupMeshComponent->SetSkeletalMesh(tempItemDefinition->worldMesh.Get());
				// And set the sphere's collision radius
				sphereComponent->SetSphereRadius(32.f);

				UE_LOG(LogTemp, Display, TEXT("APickupBase::PostEditChangeProperty(): Successfully updated the Pickup Actor!"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("APickupBase::PostEditChangeProperty(): Could not find data!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("APickupBase::PostEditChangeProperty(): GET_MEMBER_NAME_CHECKED() failed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APickupBase::PostEditChangeProperty(): Pickup Data Table not loaded properly!"));
	}
}