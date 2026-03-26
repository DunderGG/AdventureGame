#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EAbilityInputId : uint8
{
	None			UMETA(DisplayName = "None"),		// 0,  None
	Confirm			UMETA(DisplayName = "Confirm"),		// 1,  Confirm
	Cancel			UMETA(DisplayName = "Cancel"),		// 2,  Cancel
	Ability1		UMETA(DisplayName = "Ability1"),	// 3,  LMB
	Ability2		UMETA(DisplayName = "Ability2"),	// 4,  RMB
	Ability3		UMETA(DisplayName = "Ability3"),	// 5,  Q
	Ability4		UMETA(DisplayName = "Ability4"),	// 6,  E
	Ability5		UMETA(DisplayName = "Ability5"),	// 7,  R
	Sprint			UMETA(DisplayName = "Sprint"),		// 8,  Sprint
	Jump			UMETA(DisplayName = "Jump")			// 9,  Jump
};
