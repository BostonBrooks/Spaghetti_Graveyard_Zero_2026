#include "games/game0/maps/systems_test/entity_spawner/multiline.h"
//typedef bbFlag bbParseFunction(void* spawner, char* line);


#include "engine/data/bbConstants.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"
#include "engine/logic/bbFlag.h"
#include "moveables/moveables.h"

bbFlag bbPF_NewEntity(void *Spawner, char *line) {
    bbEntitySpawner *spawner = (bbEntitySpawner *) Spawner;
    char key[KEY_LENGTH];
    char name[KEY_LENGTH];
    sscanf(line, "%32[^','],%32[^,\t\n]", key, name);
    bbECS_entity *entity;
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

bbFlag bbPF_AddServerHandle(void *Spawner, char *line) {
    bbEntitySpawner *spawner = (bbEntitySpawner *) Spawner;
    bbECS_entity *entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    I32 index;

    sscanf(line, "%32[^','],%d", key, &index);

    bbHandle server_handle;
    server_handle.bloated.index = index;
    server_handle.bloated.collision = 193;

    bbCoreSynchronous_setServerEntity(&home.core.core,
                                      entity_handle,
                                      server_handle,
                                      bbInstructionSource_norewind,
                                      no_handle);
}

bbFlag bbPF_AddMovement(void *Spawner, char *line) {
    bbEntitySpawner *spawner = (bbEntitySpawner *) Spawner;
    bbECS_entity *entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    char state_str[KEY_LENGTH];
    sscanf(line, "%32[^','],%32[^,\t\n]", key, state_str);

    bbHandle state;
    bbDictionary_lookup(spawner->states, state_str, &state);


    switch (state.u64) {
        case bbMoveableType_Following:
            //Use server handle to look up target and then set goal moveable
            {
            bbMapCoords position;
            bbHandle goal_server_handle;
            I32 goal_index,speed, radius, mass;
            bbHandle moveable_handle;

            sscanf(line, "%32[^','],%32[^','], %d, %d, %d, %d, %d, %d, %d",
       key, state_str, &position.i, &position.j, &position.k, &goal_index, &speed, &radius, &mass);

            bbHandle server_handle;
            server_handle.bloated.index = goal_index;
            server_handle.bloated.collision = 193;

            bbCoreSynchronous_spawnTestMoveable(&home.core.core,
                                    entity_handle,
                                    &moveable_handle,
                                    position,
                                    speed,
                                    radius,
                                    mass,
                                    bbInstructionSource_norewind,
                                    no_handle);

            bbMoveable_setGoalMoveable(&home.ECS.moveables,moveable_handle, server_handle);

            bbTest()

            bbECS_entity* target_entity;
            bbHandle_mapComponent(home.ECS.ECS,bbECS_ServerEntities,server_handle,bbECS_ECS,NULL,(bbComponent**)&target_entity);
            bbDebug("trying to target entity %s\n", target_entity->key);
            }
            break;

        case bbMoveableType_Moving: {
            bbMapCoords position, goal;
            I32 speed, radius, mass;
            bbHandle moveable_handle;

            sscanf(line, "%32[^','],%32[^','], %d, %d, %d, %d, %d, %d, %d, %d, %d",
                   key, state_str, &position.i, &position.j, &position.k, &goal.i, &goal.j, &goal.k, &speed, &radius,
                   &mass);

            bbCoreSynchronous_spawnTestMoveable(&home.core.core,
                                                entity_handle,
                                                &moveable_handle,
                                                position,
                                                speed,
                                                radius,
                                                mass,
                                                bbInstructionSource_norewind,
                                                no_handle);

            bbMoveable_setGoalPoint(&home.ECS.moveables, moveable_handle, goal);
        }
        break;


        case bbMoveableType_Idle: {
            bbMapCoords position;
            I32 speed, radius, mass;
            bbHandle moveable_handle;

            sscanf(line, "%32[^','],%32[^','], %d, %d, %d, %d, %d, %d",
                   key, state_str, &position.i, &position.j, &position.k, &speed, &radius, &mass);

            bbCoreSynchronous_spawnTestMoveable(&home.core.core,
                                                entity_handle,
                                                &moveable_handle,
                                                position,
                                                speed,
                                                radius,
                                                mass,
                                                bbInstructionSource_norewind,
                                                no_handle);
            bbCI_Moveable_setIdle(&home.core.core, moveable_handle, bbInstructionSource_norewind, no_handle);
        }
        break;
    }
}


///This should create a graphics system component but not a graphics object;
///what is theres no graphics context for this map?
///only the graphics system works directly with
bbFlag bbPF_AddGraphics(void *Spawner, char *line) {
    bbEntitySpawner *spawner = (bbEntitySpawner *) Spawner;
    bbECS_entity *entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    char type[KEY_LENGTH];
    I32 state;

    bbMapCoords map_coords;

    sscanf(line, "%32[^','],%32[^','],%d,%d,%d,%d",
           key, type,&state, &map_coords.i, &map_coords.j, &map_coords.k);


    U64 deterministic_seed = bbArith64_hash(1234567);

    bbGraphicsComponent_data data;
    data.entity_handle = entity_handle;
    data.MC = map_coords;
    data.last_state_change = 0;
    data.last_wander_time = 0;
    data.random_seed = bbRand();
    data.drawable_state = state;
    data.drawable_type = bbGraphicsType_unit;
    bbCS_spawnGraphicsComponent(&home.core.core,
                                NULL,
                                type,
                                &data,
                                bbInstructionSource_norewind,
                                no_handle);

    return bbSuccess;
}

bbFlag bbPF_AddAI(void *spawner, char *line) {
    bbNotImplemented()
}

bbFlag bbPF_AddTeam(void *spawner, char *line) {
    bbNotImplemented()
}

bbFlag bbPF_AddHitpoints(void *spawner, char *line) {
    bbNotImplemented()
}

bbFlag bbPF_SetPlayer(void *spawner, char *line) {
    bbNotImplemented()
}


bbFlag bbPF_AddProjectile(void* Spawner, char* line)  {
    bbEntitySpawner *spawner = (bbEntitySpawner *) Spawner;
    bbECS_entity *entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    char type[KEY_LENGTH];
    I32 state;

    bbMapCoords start_point;
    bbMapCoords end_point;

    sscanf(line, "%32[^','],%32[^','],%d,%d,%d,%d,%d,%d",
           key, type, &start_point.i, &start_point.j, &start_point.k,&end_point.i, &end_point.j, &end_point.k);

    I64 delta_i = end_point.i - start_point.i;
    I64 delta_j = end_point.j - start_point.j;
    I64 delta_k = end_point.k - start_point.k;
    I64 distance = bbArith64_sqrt2(delta_i*delta_i+delta_j*delta_j+delta_k*delta_k);
    I64 start_time = 0;
    I64 end_time = 300 * distance / POINTS_PER_SQUARE;


    // bbGraphicsComponent_data data;

    //
    // bbUI_Inbox_NewArrow(&home.UI.inbox, &data);

    U64 deterministic_seed = bbArith64_hash(1234567);

    bbGraphicsComponent_data data;
    data.start_point = start_point;
    data.end_point = end_point;
    data.start_time = start_time;
    data.end_time = end_time;
    data.drawable_type = bbGraphicsType_projectile;
    data.entity_handle = entity_handle;
    bbCS_spawnGraphicsComponent(&home.core.core,
                                NULL,
                                type,
                                &data,
                                bbInstructionSource_norewind,
                                no_handle);

    return bbSuccess;
}


bbFlag bbPF_AddProjectileMovement(void* Spawner, char* line) {
    bbEntitySpawner *spawner = (bbEntitySpawner *) Spawner;
    bbECS_entity *entity = spawner->current_entity;
    bbHandle entity_handle = entity->component.entity_handle;

    char key[KEY_LENGTH];
    char type[KEY_LENGTH];
    I32 state;

    bbMapCoords start_point;
    bbMapCoords end_point;

    sscanf(line, "%32[^','],%32[^','],%d,%d,%d,%d,%d,%d",
           key, type, &start_point.i, &start_point.j, &start_point.k,&end_point.i, &end_point.j, &end_point.k);

    I64 delta_i = end_point.i - start_point.i;
    I64 delta_j = end_point.j - start_point.j;
    I64 delta_k = end_point.k - start_point.k;
    I64 distance = bbArith64_sqrt2(delta_i*delta_i+delta_j*delta_j+delta_k*delta_k);
    I64 start_time = 0;
    I64 end_time = 300 * distance / POINTS_PER_SQUARE;

    bbProjectileMovement_data data;

    data.start_coords = start_point;
    data.current_coords = start_point;
    data.end_coords = end_point;
    data.start_time = start_time;
    data.end_time = end_time;
    data.gravity = 1000;

    bbCS_spawnProjectileMovement(&home.core.core,
                             entity_handle,
                             NULL,
                             &data,
                             bbInstructionSource_norewind,
                             no_handle);


    return bbSuccess;

}