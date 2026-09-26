#ifndef BB_BBRENDERUNITS
#define BB_BBRENDERUNITS

#include "../viewport/bbUnits.h"
#include "../viewport/bbViewportApp.h"

///Render units are like ordinary units except they only exist when they're on screen
// They are able to move around like a bbMoveable component
#define UNITS_PER_GROUP 12

typedef enum
{
    bbRU_movementType_rigid,
} bbRenderUnit_movementType;

typedef struct
{
    bbMinimalDrawable md;
    I32 index;
    bbRenderUnit_movementType movement_type;
    bbDrawable* owner;
} bbRenderUnit;

typedef struct bbRenderUnitGroup
{
    bbRenderUnit units[UNITS_PER_GROUP];
    bbListElement_Handle list_element;
} bbRenderUnitGroup;

typedef bbFlag bbRenderUnitGroup_spawn_fn(struct bbRenderUnits* render_units, bbDrawable* drawable);

typedef struct bbRenderUnits
{
    bbVPool* pool;
    bbList list;

    I32 spawnFunction_num;
    bbDictionary* spawnFunction_dict;
    bbRenderUnitGroup_spawn_fn** spawnFunctions;

    ///notify drawables and units if they go on or off screen
    I32 notifyVisible_num;
    bbDictionary* notifyVisible_dict;
    bbDrawable_notifyVisible_fn** notifyVisible_fns;


} bbRenderUnits;

bbFlag bbRenderUnits_new(bbRenderUnits** render_units);

///calculate positions based on positions of drawable and other units.
bbFlag bbRenderUnits_updateMovement(bbRenderUnits* render_units);
bbFlag bbRenderUnits_populateFunctions (bbRenderUnits* render_units);
bbFlag bbRenderUnits_addSpawnFunction(bbRenderUnits* render_units,
                                      bbRenderUnitGroup_spawn_fn* spawn_function,
                                      char* key);
bbFlag bbRenderUnitGroup_spawn(bbRenderUnits* render_units,
                                      I32 spawn_function_index,
                                      bbDrawable* drawable);
bbFlag bbRenderUnitGroup_delete(bbRenderUnits* render_units,
                                      bbRenderUnitGroup* group);

///Take a drawable and render it's render units
bbFlag bbDrawFunction_renderUnits(void* drawable, void* frameDescriptor, void* cl);


bbFlag bbRenderUnitGroup_spawn_foxes(bbRenderUnitGroup** Group,
                                     bbRenderUnits* render_units,
                                     bbDrawable* drawable,
                                     bbGraphicsApp* graphics);
#endif // BB_BBRENDERUNITS