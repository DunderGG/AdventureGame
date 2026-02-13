// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(BlueprintType, Blueprintable)
class GAMEONE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the projectile collides with another object
	UFUNCTION()
	void onHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	// Remove the projectile from the level
	UFUNCTION()
	void destroyMe();

	// Mesh of the projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Mesh")
	TObjectPtr<UStaticMeshComponent> meshComponent;

	// Despawn after two seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Lifespan")
	float lifespan = 2.0f;

	//The amount of force this projectile imparts on objects it collides with
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Physics")
	float force = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = "Projectile | Components")
	TObjectPtr<USphereComponent> collisionComponent;

	// The movement component will handle movement logic.
	//	Calculates where its parent Actor should go based on velocity, gravity, and other variables.
	//	Then, during tick, it applies the movement to the projectile.
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = "Projectile | Components")
	TObjectPtr<UProjectileMovementComponent> movementComponent;

	// Timer handle for the destroyMe() timer
	FTimerHandle destroyTimerHandle;
};
