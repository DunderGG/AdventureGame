// Copyright dunder.gg. All Rights Reserved.

#include "AdventureGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace AdventureGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_Kick, "Event.Montage.Kick", "Playing montage Kick");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Ability_Kick, "Gameplay.Ability.Kick", "Using ability Kick");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Cooldown_Kick, "Gameplay.Cooldown.Kick", "Ability Kick on cooldown");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Magnitude_Duration, "Gameplay.Magnitude.Duration", "Don't remember what this is");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_State_IsBaseRecoveringHealth, "Gameplay.State.IsBaseRecoveringHealth", "Is the base health recovery active or not.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_State_IsBaseRecoveringStamina, "Gameplay.State.IsBaseRecoveringStamina", "Is the base stamina recovery active or not.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_State_IsKicking, "Gameplay.State.IsKicking", "Are we currently Kicking?");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_State_IsInAir, "Gameplay.State.IsInAir", "Are we currently in the air?");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Kick, "GameplayCue.Kick", "Don't remember...");

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& tagManager = UGameplayTagsManager::Get();
		FGameplayTag foundTag = tagManager.RequestGameplayTag(FName(*TagString), false);

		// Could not find an exact match, try to find a partial match if allowed.
		if (!foundTag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer gameplayTags;
			tagManager.RequestAllGameplayTags(gameplayTags, true);

			for (const FGameplayTag& tag : gameplayTags)
			{
				if (tag.ToString().Contains(TagString))
				{
					UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *tag.ToString());
					foundTag = tag;
					break;
				}
			}
		}

		return foundTag;
	}
}