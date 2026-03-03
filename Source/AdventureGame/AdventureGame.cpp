// Copyright dunder.gg. All Rights Reserved.

#include "AdventureGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, AdventureGame, "AdventureGame" );

DEFINE_LOG_CATEGORY(LogAdventureGame)

// Dunno about defining it here. Having to include AdventureGame.h everywhere is kinda ugly...
DEFINE_LOG_CATEGORY(LogConstructors)
/*
* TODO: Not sure what goes in here.
*/
