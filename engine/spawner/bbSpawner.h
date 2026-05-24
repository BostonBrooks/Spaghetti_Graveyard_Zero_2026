///The spawner has two parts, one that spawns entities in the main thread and
///one that spawns entities in the graphics thread. Both spawners read from the
///same file, so entities are spawned in pairs. An entity in the main thread will
///know how to send a message to an entity in the graphics thread and vice-versa.

#ifndef BB_SPAWNER_H
#define BB_SPAWNER_H


#include "engine/logic/bbFlag.h"
#include "../logic/bbDictionary.h"
#include "../logic/bbIntTypes.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/userinterface/bbUI_Inbox.h"
typedef bbFlag bbParseFunction (char* string);

typedef bbFlag bbSpawnFunction (...);

///Used to spawn an entity during gameplay. does not care about keeping the core synchronised;
///calls the function bbUI_Inbox_NewUnit(&home.UI.inbox, 0, MC, entity_index, moveable_index);
typedef bbFlag bbEntity_new(bbAgent** agent, I32 type_index, bbMapCoords MC,bbMapCoords goal_coords, I32 moveable_index, I32 entity_index);

///This function is called by bbUI_Inbox_NewUnit(&home.UI.inbox, 0, MC, entity_index, moveable_index);
typedef bbFlag bbUIUnit_new( bbMapCoords MC, I32 moveable_index, I32 entity_index);

//bbFlag bbUI_Inbox_NewUnit(bbUI_Inbox* inbox, I32 type_index, bbMapCoords MC, I32 entity_index, I32 moveable_index);
bbFlag bbUIUnit_newUnit(I32 type_index, bbMapCoords MC, I32 moveable_index, I32 entity_index);

typedef struct
{
    bbParseFunction** parse_core;
    bbParseFunction** parse_graphics;
    I32 parse_functions_available;
    bbDictionary* parse_dict;

    bbSpawnFunction** spawn_core;
    bbSpawnFunction** spawn_graphics;
    I32 spawn_functions_available;
    bbDictionary* spawn_dict;

    bbEntity_new** entity_new;
    bbUIUnit_new** unit_new;
    I32 entity_new_functions_available;
    bbDictionary* entity_new_dict;

} bbSpawner;


///Get ready to load spawn functions into memory
bbFlag bbSpawner_init(bbSpawner* spawner, I32 num_parsers, I32 num_entity_types);

///Load one pair of functions into memory
bbFlag bbParseFunction_add(bbSpawner* spawner,
    bbParseFunction* spawn_core, bbParseFunction* spawn_graphics, char* key );

bbFlag bbSpawnFunction_add(bbSpawner* spawner,
    bbSpawnFunction* spawn_core, bbSpawnFunction* spawn_graphics, char* key );

bbFlag bbEntityFunction_add(bbSpawner* spawner,
    bbEntity_new* new_entity, bbUIUnit_new* new_unit, char* key );
///Load spawn functions into memory
bbFlag bbSpawner_populate(bbSpawner* spawner);

bbFlag bbSpawner_getCore(bbSpawner* spawner,
    bbParseFunction** function, char* key);

bbFlag bbSpawner_getGraphics(bbSpawner* spawner,
    bbParseFunction** function, char* key);

bbFlag bbSpawner_spawnCore(bbSpawner* spawner, char* file_name);
bbFlag bbSpawner_spawnGraphics(bbSpawner* spawner, char* file_name);
bbFlag bbSpawner_spawnEntity(bbSpawner* spawner, bbAgent** agent, bbMapCoords MC, I32 moveable_index, I32 entity_index, char* key);
bbFlag bbSpawner_spawnEntityI(bbSpawner* spawner, bbAgent** agent, bbMapCoords MC,bbMapCoords goal_coords, I32 moveable_index, I32 entity_index, I32 type_index);
#endif //BB_SPAWNER_H