
#include "engine/ECS/graphics_system/bbGraphicsSystem.h"

#include "engine/logic/bbBloatedPool.h"


bbFlag bbGraphicsSystem_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbGraphicsSystem_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle);


bbFlag bbGraphicsSystem_init(bbGraphicsSystem* graphics_system, bbECS* ECS)
{
    bbVPool_newBloated(&graphics_system->system.pool, sizeof(bbGraphicsComponent), 1000, 10, "GRAPHICS SYSTEM");

    graphics_system->system.getComponent = bbGraphicsSystem_getComponent_fn;
    graphics_system->system.getHandle = bbGraphicsSystem_getHandle_fn;
    graphics_system->system.ECS = ECS;

    ECS->systems[bbECS_Graphics] = (bbSystem* )graphics_system;

    graphics_system->spawn_functions = calloc(193, sizeof(bbGraphics_spawnFunction*));
    graphics_system->spawn_function_count = 0;
    graphics_system->max_function_count = 193;

    bbDictionary_new(&graphics_system->spawn_dict,193);
    return bbSuccess;
}

bbFlag bbGraphics_spawnFunction_add(bbGraphicsSystem* graphics_system,
    bbGraphics_spawnFunction* function, char* key) {
    I32 index = graphics_system->spawn_function_count++;
    bbAssert(index < graphics_system->max_function_count, "out of allocated space\n");
    graphics_system->spawn_functions[index] = function;
    bbHandle function_handle; function_handle.u64 = index;
    bbDictionary_add(graphics_system->spawn_dict,key, function_handle);
    return bbSuccess;

}

bbFlag bbGraphicsSystem_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool,(void**)component,component_handle);
}

bbFlag bbGraphicsSystem_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool,(void*)component,component_handle);
}

bbFlag bbCI_spawnGraphicsComponent( bbCore* core,
                                          char* type,
                                          bbMapCoords MC,
                                          I32 drawable_state,
                                          U64 random_seed,
                                          bbHandle entity,
                                          bbInstruction_source source,
                                          bbHandle action) {
    bbNotImplemented()
}

bbFlag bbCS_spawnGraphicsComponent( bbCore* core,
                                          bbGraphicsComponent** this,
                                          char* type,
                                          bbGraphicsComponent_data* data,
                                          bbInstruction_source source,
                                          bbHandle action){
    bbNotImplemented() //rollback

    bbECS* ECS = core->ECS;
    bbGraphicsSystem* system = (bbGraphicsSystem*)ECS->systems[bbECS_Graphics];
    bbGraphicsComponent* component;
    bbHandle component_handle;



    bbVPool_alloc2(system->system.pool, (void**)&component, &component_handle);

    component->data = *data;

    bbCS_entity_setComponent(core,
                 ECS,
                 data->entity_handle,
                 component_handle,
                 bbECS_Teams,
                 bbInstructionSource_internal,
                 no_handle);

    bbHandle function_handle;
    bbDictionary_lookup(system->spawn_dict,type, &function_handle);

    bbGraphics_spawnFunction* function = system->spawn_functions[function_handle.u64];
    function(system,*data);

    if (this != NULL) *this = component;

    return bbSuccess;
}

bbFlag bbI_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}
bbFlag bbI_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}