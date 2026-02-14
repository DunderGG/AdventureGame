// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "../EquippableToolBase.h"
#include "Pistol.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEONE_API APistol : public AEquippableToolBase
{
	GENERATED_BODY()
	
public:
	APistol();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void bindInputAction(const UInputAction* InputToBind) override;

	virtual void use() override;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectile> projectileClass;

protected:
	// Sound effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> shootAudioComponent;
};
