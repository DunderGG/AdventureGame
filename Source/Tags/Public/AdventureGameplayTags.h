// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace AdventureGameplayTags
{
	TAGS_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Kick);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Ability_Kick);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Cooldown_Kick);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_Magnitude_Duration);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_State_IsBaseRecoveringHealth);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_State_IsBaseRecoveringStamina);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_State_IsKicking);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_State_IsInAir);
	TAGS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Kick);
	
	// Declare all of the custom native tags that AdventureGame will use
	/*
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	FTAGS_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	FTAGS_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	FTAGS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
	*/
};