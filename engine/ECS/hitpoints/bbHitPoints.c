#include "engine/ECS/hitpoints/bbHitPoints.h"

#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbIterator.h"
#include "engine/logic/bbSystemPool.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbHitPoints_init(bbHitPoints* system,bbECS* ECS) {
    bbVPool_newSystem(&system->system.pool,bbECS_Hitpoints,sizeof(bbHitPoint),10,1000,"HITPOINTS");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbHitPoint,list),NULL,bbECS_Hitpoints);

    system->system.getComponent = bbHitPoints_getComponent;
    system->system.getHandle = bbHitPoints_getHandle;
    system->system.delete = bbHitPoints_delete;

    system->system.ECS = ECS;
    ECS->systems[bbECS_Hitpoints] = (bbSystem*)system;
    return bbSuccess;
}
bbFlag bbHitPoints_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbHitPoints_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}
bbFlag bbHitPoints_delete(struct bbSystem* system, bbHandle component_handle)
{
    bbHitPoints* hitpoints = (bbHitPoints*)system;
    bbHitPoint* component;
    bbHitPoints_getComponent(system, (bbComponent**)&component, component_handle);
    bbList_remove(&hitpoints->list,component);
    bbVPool_free(hitpoints->system.pool,component);

    return bbSuccess;
}

bbFlag bbCS_Hitpoints_update(bbCore* core,
                             bbECS* ECS,
                             bbInstruction_source source,
                             bbHandle action) {
    bbNotImplemented()
}

bbFlag bbCI_Hitpoints_update(bbCore* core,
                             bbECS* ECS,
                             bbInstruction_source source,
                             bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_Hitpoints_update;
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag update_hitpoints_fn(bbList* list, void* node, void* cl) {
    bbHitPoint* hitpoint = (bbHitPoint*)node;

    if (hitpoint->prev_health != hitpoint->current_health) {
        float new_HP = (float)hitpoint->current_health / (float)hitpoint->max_health;
        bbUI_Inbox_SetUnitHP(&home.UI.inbox,hitpoint->component.entity_handle,new_HP);
        bbDebug("new_HP = %f\n", new_HP);
    }
    hitpoint->prev_health = hitpoint->current_health;
    return bbContinue;
}

bbFlag bbI_Hitpoints_update_fn(bbCore* core, bbInstruction* instruction) {

    bbHitPoints* hitpoints = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbIterator iterator = bbIterator_new(&hitpoints->list);
    bbIterator_mapL(&iterator, update_hitpoints_fn,NULL);
}
bbFlag bbI_Hitpoints_unupdate_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented() //I guess this is what we want?

    bbHitPoints* hitpoints = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbIterator iterator = bbIterator_new(&hitpoints->list);
    bbIterator_mapL(&iterator, update_hitpoints_fn,NULL);
}

bbFlag bbCS_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbHitPoint** hitpoint,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action) {
    bbNotImplemented() //rollback

    bbHitPoints* hitpoints = (bbHitPoints*)ECS->systems[bbECS_Hitpoints];
    bbHitPoint* component;
    bbHandle component_handle;
    bbVPool_alloc2(hitpoints->system.pool,(void**)&component,&component_handle);

    component->max_health = max_hitpoints;
    component->current_health = max_hitpoints;
    component->prev_health = max_hitpoints;
    component->component.entity_handle = entity;

    bbCS_entity_setComponent(core,
                         ECS,
                         entity,
                         component_handle,
                         bbECS_Hitpoints,
                         bbInstructionSource_internal,
                         no_handle);

    bbList_pushL(&hitpoints->list,component);

    if (hitpoint != NULL) *hitpoint = component;


    return bbSuccess;
}

bbFlag bbCI_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action) {
    bbNotImplemented()
}

bbFlag bbI_Hitpoints_spawn_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}
bbFlag bbI_Hitpoints_unspawn_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}

bbFlag bbCS_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action) {
    bbNotImplemented()
}

bbFlag bbCI_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action) {
    allocActiveInstruction(instruction)
    instruction->type = bbI_Hitpoints_damage;

    instruction->data.damage_agent.agent = entity;
    instruction->data.damage_agent.hitpoints = hitpoints;
    instruction->data.damage_agent.damage_type = damageType;

    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbI_Hitpoints_damage_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented() //rollback

    bbHitPoints* hit_points = (bbHitPoints*)core->ECS->systems[bbECS_Hitpoints];
    bbHandle entity_handle = instruction->data.damage_agent.agent;
    I32 damage = instruction->data.damage_agent.hitpoints;
    I32 damageType = instruction->data.damage_agent.damage_type;

    bbHitPoint* hitpoint;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Hitpoints,NULL,(bbComponent**)&hitpoint);

    bbWarning(hitpoint!=NULL,"Damaging an entity with no hitpoint compnent\n");
    if (hitpoint != NULL) {
        hitpoint->current_health -= damage;
    }

    return bbSuccess;
}
bbFlag bbI_Hitpoints_undamage_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}