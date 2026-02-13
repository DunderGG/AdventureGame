// Copyright dunder.gg. All Rights Reserved.

#include "EquippableToolBase.h"
#include "MainCharacter.h"

// Sets default values
AEquippableToolBase::AEquippableToolBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create this tool's mesh component and make sure it worked
	toolMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("toolMesh"));
	check(toolMeshComponent != nullptr);
}

// Called when the game starts or when spawned
void AEquippableToolBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// We add logic for these when creating subclasses of EquippableToolBase,
//	such as launching a projectile or opening a chest
void AEquippableToolBase::use()
{
}
void AEquippableToolBase::bindInputAction(const UInputAction* actionToBind)
{
}

// Called every frame
void AEquippableToolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}