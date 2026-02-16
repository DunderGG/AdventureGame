// Copyright dunder.gg. All Rights Reserved.
 
#pragma once

//#include "CoreMinimal.h"
//#include ECreatureType.generated.h

UENUM(BlueprintType)
enum class ECreatureType : uint8
{
	CT_DEFAULT UMETA(DisplayName = "Default"),
	CT_HUMAN UMETA(DisplayName = "Human"),
	CT_SMALLANIMAL UMETA(DisplayName = "Small Herbivore"),
	CT_SMALLCARNIVORE UMETA(DisplayName = "Small Carnivore"),
	CT_MEDIUMANIMAL UMETA(DisplayName = "Medium Herbivore"),
	CT_MEDIUMCARNIVORE UMETA(DisplayName = "Medium Carnivore"),
	CT_LARGEANIMAL UMETA(DisplayName = "Large Herbivore"),
	CT_LARGECARNIVORE UMETA(DisplayName = "Large Carnivore"),
	CT_MONSTER UMETA(DisplayName = "Monsterous")
};