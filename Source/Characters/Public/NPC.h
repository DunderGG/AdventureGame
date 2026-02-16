// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "NPC.generated.h"

// We make it abstract so we don't make new blueprints from the base class
UCLASS(Abstract, NotBlueprintable)
class CHARACTERS_API ANPC : public ACharacterBase
{
	GENERATED_BODY()
	
};
