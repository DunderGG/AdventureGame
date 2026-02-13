// Copyright dunder.gg. All Rights Reserved.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComponent"));
	check(collisionComponent != nullptr);
	collisionComponent->InitSphereRadius(5.0f);

	// Create the mesh component
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComponent"));
	check(meshComponent != nullptr);
	meshComponent->SetupAttachment(collisionComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComponent"));
	check(movementComponent != nullptr);
	
	// Set the collision profile to the "Projectile" preset
	//	The default behaviour of the "Projectile" collision profile is Block, which
	//	creates collisions on any object it collides with
	//		In UE, you can find them under Project Settings -> Engine -> Collision
	// You have to actually set up a new collision profile if you want to do this, DON'T FORGET
	collisionComponent->BodyInstance.SetCollisionProfileName("Projectile");

	// When a collision occurs, or the projectile hits something, call the onHit() function
	collisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::onHit);

	// Set the collisionComponent as the projectile's RootComponent
	RootComponent = collisionComponent;
	// and as the UpdatedComponent for the movement component to track
	movementComponent->UpdatedComponent = collisionComponent;

	// Initialize the movementComponent properties
	movementComponent->InitialSpeed = 3000.0f;
	movementComponent->MaxSpeed = 3000.0f;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bShouldBounce = true;

	// Set the initial lifespan to dissapear after 2 seconds by default
	//	This is a property inherited from AActor. 0 would mean the actor lives until the game stops.
	InitialLifeSpan = lifespan;

	// Make the projectile go away after 2 seconds
	//UE_LOG(LogTemp, Warning, TEXT("AProjectile::AProjectile(): Projectile Spawned"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld() != nullptr)
	{
		// If we put this in the constructor, it starts running while game is not even running...
		GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle, this, &AProjectile::destroyMe, 1, false, -1);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::onHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	if (otherActor != nullptr)
	{
		if (otherActor != this)
		{
			if (otherComp != nullptr)
			{
				if (otherComp->IsSimulatingPhysics())
				{
					//UE_LOG(LogTemp, Display, TEXT("AProjectile::onHit(): Projectile hit something"));
					// If everything checks out, add an impulse to the other component, 
					//	with the velocity of the projectile multiplied by its force,
					//		and apply it at the location of the projectile actor
					otherComp->AddImpulseAtLocation(GetVelocity() * force, GetActorLocation());

					//After impact, destroy our projectile
					// Changed to be destroyed shortly after impact instead
					//Destroy();
				}
				else
				{
					//UE_LOG(LogTemp, Display, TEXT("AProjectile::onHit(): otherComp has no physics"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AProjectile::onHit(): otherComp is nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AProjectile::onHit(): projectile collided with itself???"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AProjectile::onHit(): otherActor is nullptr"));
	}
}

void AProjectile::destroyMe()
{
	//UE_LOG(LogTemp, Display, TEXT("AProjectile::destroyMe: Destroying projectile"));
	Destroy();
}