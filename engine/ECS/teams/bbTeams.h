#ifndef BB_TEAMS_H
#define BB_TEAMS_H
#include "engine/ECS/bbECS.h"

typedef enum {
    bbTeam_player,
    bbTeam_monster,
    bbTeam_numTeams
};

typedef struct {
    bbComponent component;
    I32 team;
} bbTeam;

typedef struct {
    bbSystem system;

    //Team alignment data
} bbTeams;

bbFlag bbTeams_init(bbTeams* system, bbECS* ECS);


bbFlag bbCI_spawnTeamComponent(bbCore* core,
                             bbHandle entity,
                             I32 team,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_spawnTeamComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnTeamComponent_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCI_spawnTeamComponent(bbCore* core,
                             bbHandle entity_handle,
                             I32 team,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCS_spawnTeamComponent(bbCore* core,
                             bbHandle entity_handle,
                             bbTeam** this,
                             I32 team_number,
                             bbInstruction_source source,
                             bbHandle action);


#endif //BB_TEAMS_H