#include "engine/data/bbHome.h"
#include "engine/ECS/bbEntitySpawner.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "games/game0/maps/skellychase/entity_spawner/skelly.h"

I32 parse_function_count = 193;
I32 spawn_function_count = 194;
I32 unspawn_function_count = 195;

bbFlag bbPF_null(void* spawner, char* string)
{
    printf("%s\n", string);
    return bbSuccess;
}


bbFlag bbPF_tree(void* spawner, char* string)
{
    char key[KEY_LENGTH];
    char tree_type[KEY_LENGTH];
    bbMapCoords position;
    sscanf ( string, "%[^','],%[^','],%d,%d", key, tree_type,&position.i,&position.j);

    position.k = bbMapCoords_getElevation(&home.ground_surface, position);

    bbUI_Inbox_NewTree(&home.UI.inbox,tree_type, position);

    return bbSuccess;
}

bbFlag bbPF_standard(void* Spawner, char* string)
{
    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    bbSpawnFunctionArgs args;
    I32 num_chars;
    char spawn_functions[256];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d,%d,%d,%d,%d,%n",
        key,entity_type,&args.position.i,&args.position.j,&args.position.k,
        &args.goalpoint.i,&args.goalpoint.j,&args.goalpoint.k,&args.server_handle.bloated.index,
        &args.server_handle.bloated.collision,&num_chars);
    //bbDebug("%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,",
    //key,entity_type,position.i,position.j,position.k,
   // goalpoint.i,goalpoint.j,goalpoint.k,server_handle.bloated.index,
    //server_handle.bloated.collision);



    char component[KEY_LENGTH];
    I32 component_length;
    while (1)
    {
        char separator;
        sscanf(&string[num_chars], "%[^',\n']%c%n",component,&separator,&component_length);
        if (separator == '\0' || separator == '\n') break;
        num_chars += component_length;
        //printf("%s,",component);


        bbSpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->spawn_dict, component, &handle);
        spawn_function = spawner->spawn_functions[handle.u64];

        spawn_function(spawner, NULL, args,bbInstructionSource_norewind);
    }

    printf("\n");

    return bbSuccess;
}

bbFlag bbSF_null(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbDebug("position = (%d, %d, %d)\n"
            "goalpoint = (%d, %d, %d)\n"
            "server_handle = (%d, %d)\n"
            "source = %d\n",
            args.position.i, args.position.j, args.position.k,
            args.goalpoint.i, args.goalpoint.j, args.goalpoint.k,
            args.server_handle.bloated.index, args.server_handle.bloated.collision,
            source);
}




bbFlag bbEntitySpawner_populate(bbEntitySpawner* spawner)
{
    bbParseFunction_add(spawner, bbPF_null, "NULL");
    bbParseFunction_add(spawner, bbPF_standard, "STANDARD");
    bbParseFunction_add(spawner, bbPF_tree, "TREE");
    bbParseFunction_add(spawner, bbPF_skellyParser, "SKELLY");
    bbParseFunction_add(spawner, bbPF_skelly2Parser, "SKELLY2");
    bbSpawnFunction_add(spawner, bbSF_null, "NULL");

    bbSpawnFunction_add(spawner, bbSF_addServerEntity_skelly, "SKELLY_SERVER");
    bbSpawnFunction_add(spawner, bbSF_addMoveable_skelly, "SKELLY_MOVEABLE");
    bbSpawnFunction_add(spawner, bbSF_addMoveable_skelly2, "SKELLY_MOVEABLE2");
    bbSpawnFunction_add(spawner, bbSF_addGraphics_skelly, "SKELLY_GRAPHICS");
    return bbSuccess;
}