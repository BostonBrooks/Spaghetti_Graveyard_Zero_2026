#include "ai_instructions.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/logic/bbIntTypes.h"
#include "ai_null.h"
#include "entity_spawner/live_spawn.h"
#include "games/game0/maps/systems_test/core/player_goalpoint.h"
#include "games/game0/maps/systems_test/AI_system/ai_player.h"



bbFlag bbAI_Update_Fireball(bbAI_Component* component)
{
    //if close to goalpoint

    switch (component->state)
    {
    case bbAIState_Idle:
        {
            bbMoveable* moveable;
            bbHandle moveable_handle;
            bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                     bbECS_Moveables, &moveable_handle,
                                     (bbComponent**)&moveable);

            I64 delta_i = (moveable->goalpoint.i - moveable->position.i);
            I64 delta_j = (moveable->goalpoint.j - moveable->position.j);

            int64_t distance_squared = delta_i * delta_i + delta_j * delta_j;

            if (distance_squared < POINTS_PER_TILE * POINTS_PER_TILE)
            {
                bbHandle entity_handle;

                bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                         bbECS_ECS, &entity_handle,
                         (bbComponent**)&moveable);

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_attacking);

                bbCI_Moveable_setDead(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);

                bbHandle AI_handle;

                bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);


                bbPlayers* players = &home.ECS.players;
                bbHandle player_character = players->players[players->this_player].selected_entities[0];

                bbCI_AI_setStriking(&home.core.core,
                                       AI_handle,
                                       player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    case bbAIState_Striking:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {
                bbHandle entity_handle;

                bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                         bbECS_ECS, &entity_handle,NULL);
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_dead);



                bbHandle AI_handle;
                bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);

                //TODO set dead
                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);

                bbCoreInput_entity_deleteEntity(&home.core.core,
                                       home.ECS.ECS,
                                       entity_handle,
                                 bbInstructionSource_internal, no_handle);
            }
            break;
        }
    }

    return bbSuccess;
}

bbFlag bbAI_Update_Chase(bbAI_Component* component)
{
    //bbHere()
    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* player_moveable;
    bbHandle player_handle;


    bbPlayers* players = &home.ECS.players;
    bbHandle player_character = players->players[players->this_player].selected_entities[0];

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          player_character, bbECS_Moveables,
                          &player_handle, (bbComponent**)&player_moveable);

    if (moveable_handle.bloated.index == player_handle.bloated.index)
        return bbSuccess;

    U64 distance_squared = (moveable->position.i - player_moveable->position.i)
        * (moveable->position.i - player_moveable->position.i)
        + (moveable->position.j - player_moveable->position.j)
        * (moveable->position.j - player_moveable->position.j);

    if (moveable->type == bbMoveableType_Idle)
    {
        if (distance_squared > POINTS_PER_TILE * POINTS_PER_TILE * 200)
            return bbSuccess;

        if (component->last_state_change > home.core.core.simulation_time - 60) return bbSuccess;

        bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                     player_handle,
                                     bbInstructionSource_internal, no_handle);


    }



    if (moveable->type == bbMoveableType_Following)
    {
        if (distance_squared < POINTS_PER_TILE * POINTS_PER_TILE * 8) {
            bbCI_Moveable_setIdle(&home.core.core,
                             moveable_handle,
                             bbInstructionSource_internal, no_handle);

            bbHandle AI_handle;

            bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component,&AI_handle);

            return bbSuccess;

        }

        if (distance_squared < POINTS_PER_TILE * POINTS_PER_TILE * 160) return
            bbSuccess;

        bbCI_Moveable_setIdle(&home.core.core,
                                     moveable_handle,
                                     bbInstructionSource_internal, no_handle);


    }
    return bbSuccess;
}

bbFlag bbAI_Update_Striking(bbAI_Component* component)
{
    //bbHere()

    bbHandle AI_handle;
    bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);


    bbHandle entity_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &entity_handle,
                             NULL);

    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* player_moveable;
    bbHandle player_handle;

    bbPlayers* players = &home.ECS.players;
    bbHandle player_character = players->players[players->this_player].selected_entities[0];

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          player_character, bbECS_Moveables,
                          &player_handle, (bbComponent**)&player_moveable);


    I64 delta_i = (player_moveable->position.i - moveable->position.i);
    I64 delta_j = (player_moveable->position.j - moveable->position.j);
    I64 distance = bbArith64_sqrt2(delta_i * delta_i + delta_j * delta_j);


    //bbAIState_Idle,
    //bbAIState_Approaching,
    //bbAIState_Striking,
    //bbAIState_Recovering,

    switch (component->state)
    {
    case bbAIState_Idle:
        {
            if (distance > POINTS_PER_TILE * 20) return bbSuccess;

            bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

            bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                             player_handle,
                             bbInstructionSource_internal, no_handle);


        bbPlayers* players = &home.ECS.players;
        bbHandle player_character = players->players[players->this_player].selected_entities[0];
            bbCI_AI_setApproaching(&home.core.core,
                                   AI_handle,
                                   player_character,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);


            break;
        }
    case bbAIState_Approaching:
        {
            if (distance<POINTS_PER_TILE * 3)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_attacking);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);



                bbPlayers* players = &home.ECS.players;
                bbHandle player_character = players->players[players->this_player].selected_entities[0];

                bbCI_AI_setStriking(&home.core.core,
                                       AI_handle,
                                       player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            if (distance>POINTS_PER_TILE * 25)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);




                bbCI_AI_setIdle(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }
            break;
        }
    case bbAIState_Striking:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {

                //bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                 moveable_handle,
                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    case bbAIState_Recovering:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                 player_handle,
                                 bbInstructionSource_internal, no_handle);

                bbPlayers* players = &home.ECS.players;
                bbHandle player_character = players->players[players->this_player].selected_entities[0];
                bbCI_AI_setApproaching(&home.core.core,
                                       AI_handle,
                                       player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    }

    return bbSuccess;
}


bbFlag bbAI_Update_Lunging(bbAI_Component* component)
{
    //bbHere()

    bbHandle AI_handle;
    bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);

    bbHandle entity_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &entity_handle,
                             NULL);

    U64 time = home.core.core.simulation_time;

    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* target_moveable;
    bbHandle target_moveable_handle;

    bbPlayers* players = &home.ECS.players;
    bbHandle target_entity;
    bbTeams_findNearestTarget(&home.core.core, home.ECS.ECS, entity_handle , &target_entity, POINTS_PER_TILE * 1000);

    if (target_entity.u64 == home.ECS.ECS->system.pool->null.u64) {
        bbHere()
        return bbSuccess;
    }

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          target_entity, bbECS_Moveables,
                          &target_moveable_handle, (bbComponent**)&target_moveable);

    I64 delta_i = (target_moveable->position.i - moveable->position.i);
    I64 delta_j = (target_moveable->position.j - moveable->position.j);
    I64 distance = bbArith64_sqrt2(delta_i * delta_i + delta_j * delta_j);

    bbDebug("distance = %ld\n", distance/POINTS_PER_TILE);

    bbTeam *attacker_team;

    bbDebug("Attacker: index = %d, system = %d, generation = %d\n"
             "Target: index = %d, system = %d, generation = %d\n",
             entity_handle.system.index, entity_handle.system.system, entity_handle.system.generation,
             target_entity.system.index, target_entity.system.system, target_entity.system.generation);

    //bbAIState_Idle,
    //bbAIState_Approaching,
    //bbAIState_Striking,
    //bbAIState_Recovering,

    switch (component->state)
    {
    case bbAIState_Idle:
        {
            if (distance > POINTS_PER_TILE * 20) return bbSuccess;

            bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

            bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                             target_moveable_handle,
                             bbInstructionSource_internal, no_handle);


            bbCI_AI_setApproaching(&home.core.core,
                                   AI_handle,
                                   target_entity,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);
            break;
        }
    case bbAIState_Approaching:
        {
            if (distance<POINTS_PER_TILE * 10)
            {
                if (time >= component->last_attack + 120)
                {

                    bbCI_Moveable_setGoalLunging(&home.core.core,
                                     moveable_handle,
                                     target_moveable_handle,
                                     bbInstructionSource_internal, no_handle);

                    bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);


                    bbCI_AI_setStriking(&home.core.core,
                                           AI_handle,
                                           target_entity,
                                           home.core.core.simulation_time,
                                     bbInstructionSource_internal, no_handle);
                }
            }

            if (distance>POINTS_PER_TILE * 25)
            {

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);




                bbCI_AI_setIdle(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }
            break;
        }
    case bbAIState_Striking:
        {
            if (distance<POINTS_PER_TILE * 4)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                 moveable_handle,
                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }


            if (component->last_state_change < home.core.core.simulation_time - 20)
            {


                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                 moveable_handle,
                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    case bbAIState_Recovering:
        {
            if (component->last_state_change < home.core.core.simulation_time - 30)
            {

                if (distance<POINTS_PER_TILE * 10)
                {
                    if (time >= component->last_attack + 120)
                    {


                        bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                        bbCI_Moveable_setGoalLunging(&home.core.core,
                                         moveable_handle,
                                         target_moveable_handle,
                                         bbInstructionSource_internal, no_handle);



                        bbCI_AI_setStriking(&home.core.core,
                                               AI_handle,
                                               target_entity,
                                               home.core.core.simulation_time,
                                         bbInstructionSource_internal, no_handle);
                        break;
                    }
                }

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                 target_moveable_handle,
                                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setApproaching(&home.core.core,
                                       AI_handle,
                                       target_entity,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    }

    return bbSuccess;
}




bbFlag bbAI_Update_CastFireball(bbAI_Component* component)
{
    //bbHere()

    bbHandle AI_handle;
    bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);


    bbHandle entity_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &entity_handle,
                             NULL);

    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* player_moveable;
    bbHandle player_handle;

    bbPlayers* players = &home.ECS.players;
    bbHandle player_character = players->players[players->this_player].selected_entities[0];
    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          player_character, bbECS_Moveables,
                          &player_handle, (bbComponent**)&player_moveable);


    I64 delta_i = (player_moveable->position.i - moveable->position.i);
    I64 delta_j = (player_moveable->position.j - moveable->position.j);
    I64 distance = bbArith64_sqrt2(delta_i * delta_i + delta_j * delta_j);


    //bbAIState_Idle,
    //bbAIState_Approaching,
    //bbAIState_Striking,
    //bbAIState_Recovering,

    switch (component->state)
    {
    case bbAIState_Idle:
        {
            if (distance > POINTS_PER_TILE * 40) return bbSuccess;

            bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

            bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                             player_handle,
                             bbInstructionSource_internal, no_handle);


        bbPlayers* players = &home.ECS.players;
        bbHandle player_character = players->players[players->this_player].selected_entities[0];
            bbCI_AI_setApproaching(&home.core.core,
                                   AI_handle,player_character,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);




            break;
        }
    case bbAIState_Approaching:
        {
            if (distance<POINTS_PER_TILE * 20)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_attacking);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);




                bbPlayers* players = &home.ECS.players;
                bbHandle player_character = players->players[players->this_player].selected_entities[0];
                bbCI_AI_setStriking(&home.core.core,
                                       AI_handle,
                                       player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);


            }


            break;
        }
    case bbAIState_Striking:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {

                //bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                 moveable_handle,
                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);


                bbSpawnFunctionArgs args;
                args.position = moveable->position;
                args.goalpoint = player_moveable->position;
                args.speed = 18000;
                args.radius = 0;
                args.mass = 1;
                bbCI_live_spawnEntity(&home.core.core, args, "FIREBALL_LIVE", bbInstructionSource_internal, no_handle);
            }

            break;
        }
    case bbAIState_Recovering:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                 player_handle,
                                 bbInstructionSource_internal, no_handle);

                bbPlayers* players = &home.ECS.players;
                bbHandle player_character = players->players[players->this_player].selected_entities[0];
                bbCI_AI_setApproaching(&home.core.core,
                                       AI_handle,
                                       player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    }

    return bbSuccess;
}



bbFlag bbAI_Functions_populate(bbAI_Functions* self)
{
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Skelly2,"UPDATE_SKELLY");
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Player,"UPDATE_PLAYER");
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Fireball,"UPDATE_FIREBALL");
    // bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Player,"COMMAND_PLAYER");
    // bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Skelly,"COMMAND_SKELLY");


    bbAI_Functions_add(self, AI_Update, bbAI_Update_NULL, "UPDATE_NULL");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Striking, "UPDATE_STRIKING");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Lunging, "UPDATE_LUNGING");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Player, "UPDATE_PLAYER");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Fireball, "UPDATE_FIREBALL");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_CastFireball, "UPDATE_CASTFIREBALL");
    bbAI_Functions_add(self, AI_Command, bbAI_Command_NULL, "COMMAND_NULL");
    bbAI_Functions_add(self, AI_Command, bbAI_Command_Player, "COMMAND_PLAYER");
    return bbSuccess;
}
