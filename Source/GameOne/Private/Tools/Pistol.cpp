// Copyright dunder.gg. All Rights Reserved.

#include "Tools/Pistol.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacter.h"
#include "projectile.h"

APistol::APistol()
{
	shootAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("shootAudioComponent"));
	shootAudioComponent->SetupAttachment(RootComponent);
}

// Called every frame
void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APistol::use()
{
	UWorld* const world = GetWorld();

	if (world != nullptr && projectileClass != nullptr)
	{
		// Get the position the character is looking at
		FVector targetPosition = owningCharacter->getCameraTargetLocation();
		// Get the correct socket to spawn projectile from
		FVector socketLocation = toolMeshComponent->GetSocketLocation("Muzzle");
		// Define the rotation (pitch, yaw, roll) of the projectile as it spawns
		//	FindLookAtRotation calculates and returns the rotation you'd need at socketLocation to face targetPosition
		FRotator spawnRotation = UKismetMathLibrary::FindLookAtRotation(socketLocation, targetPosition);
		// Define the spawn location of the projectile. 
		//	The muzzle isn't quite at the front, so you need to multiply the vector by an offset
		FVector spawnLocation = socketLocation + UKismetMathLibrary::GetForwardVector(spawnRotation) * 10.0;

		// This class includes information about where and how to spawn the actor
		FActorSpawnParameters actorSpawnParams;

		// Override the Spawn Collision Handling
		//	This tried to find a place to spawn the projectile where it isn't colliding with another Actor (such as inside a wall)
		//	  but won't spawn it if a suitable location isn't found
		actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Finally spawn the projectile at the muzzle
		AProjectile* spawned = world->SpawnActor<AProjectile>(projectileClass, spawnLocation, spawnRotation, actorSpawnParams);
		
		// Play the sound effect of shooting
		shootAudioComponent->Play();

		//UE_LOG(LogTemp, Display, TEXT("APistol::use(): Spawned projectile at = %s"), *spawned->GetActorLocation().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed getting world"));
		return;
	}
}

void APistol::bindInputAction(const UInputAction* inputToBind)
{
	if (APlayerController* playerController = Cast<APlayerController>(owningCharacter->GetController()))
	{
		if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerController->InputComponent))
		{
			// Fire pistol
			enhancedInputComponent->BindAction(inputToBind, ETriggerEvent::Triggered, this, &APistol::use);
		}
	}
}
