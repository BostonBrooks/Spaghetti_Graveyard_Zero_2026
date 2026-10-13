#include "games/game0/maps/systems_test/entity_spawner/multiline.h"
//typedef bbFlag bbParseFunction(void* spawner, char* line);


#include "engine/data/bbConstants.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"

bbFlag bbPF_NewEntity(void* Spawner, char* line) {

    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    char key[KEY_LENGTH];
    char name[KEY_LENGTH];
    sscanf(line, "%32[^','],%32[^,\t\n]", key, name);
    bbECS_entity* entity;
    bbCoreSynchronous_spawnEmptyEntity(&home.core.core,
                                       home.ECS.ECS,
                                       &entity,
                                       key,
                                       bbInstructionSource_norewind,
                                       no_handle);


    bbHandle entity_handle = entity->component.entity_handle;

    spawner->current_entity = entity;
    spawner->entity_handle = entity_handle;

    return bbSuccess;
}
bbFlag bbPF_AddServerHandle(void* Spawner, char* line) {
    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    bbECS_entity* entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    I32 index;

    sscanf(line, "%32[^','],%d", key, &index);

    bbHandle server_handle;
    server_handle.bloated.index = index;
    server_handle.bloated.collision = 1;

    bbCoreSynchronous_setServerEntity(&home.core.core,
                                   entity_handle,
                                   server_handle,
                                  bbInstructionSource_norewind,
                                  no_handle);


}
bbFlag bbPF_AddMovement(void* spawner, char* line) {
    bbNotImplemented()
}
bbFlag bbPF_AddGraphics(void* Spawner, char* line) {
    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    bbECS_entity* entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    char type[KEY_LENGTH];

    bbMapCoords map_coords;

    sscanf(line, "%32[^','],%32[^','],%d,%d,%d",
        key, type, &map_coords.i, &map_coords.j, &map_coords.k);


    U64 deterministic_seed = bbArith64_hash(1234567);

    bbGraphicsComponent_data data;
    data.entity_handle = entity_handle;
    data.MC = map_coords;
    data.last_state_change = 0;
    data.last_wander_time = 0;
    data.random_seed = bbRand();
    data.drawable_state = bbDrawableState_idle;
    data.drawable_type = bbGraphicsType_unit;
    bbCS_spawnGraphicsComponent(&home.core.core,
                                              NULL,
                                              "COW",
                                              &data,
                                              bbInstructionSource_norewind,
                                              no_handle);

    return bbSuccess;
}
bbFlag bbPF_AddAI(void* spawner, char* line) {
    bbNotImplemented()
}
bbFlag bbPF_AddTeam(void* spawner, char* line) {
    bbNotImplemented()
}
bbFlag bbPF_AddHitpoints(void* spawner, char* line) {
    bbNotImplemented()
}
bbFlag bbPF_SetPlayer(void* spawner, char* line) {
    bbNotImplemented()
}
