///Render units are groups of 12 units that are modelled in the ECS as one entity.
///The positions of render units are calculated based off the position of the group and an offset.
///Render units from one group clip through render units of anither group.
///Here we introduce a corrective force to prevent units from occupying the same space,
///This will be not as complex as the forces used in bbMoveables.c
#ifndef BB_RENDER_UNITS_AVOIDANCE
#define BB_RENDER_UNITS_AVOIDANCE

#define AVOIDANCE_PASSES 1
#include "bbRenderUnits.h"
#include "engine/logic/bbIterator.h"



//typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);



static bbFlag sum_forces_inner(bbList* list, void* node, void* cl) {
    bbRenderUnitGroup* inner_group = node;
    bbRenderUnitGroup* outer_group = cl;

    if (inner_group == outer_group) {
        for (I32 i = 0; i < UNITS_PER_GROUP; i++) {
                bbRenderUnit* outer_unit = &outer_group->units[i];
                if (outer_unit->avoidance_type == bbRU_avoidanceType_none) continue;
                if (outer_unit->avoidance_type == bbRU_avoidanceType_rigid) continue;
            for (I32 j = 0; j < UNITS_PER_GROUP; j++) {
                if (i == j) continue;
                bbRenderUnit* inner_unit = &inner_group->units[j];
                if (inner_unit->avoidance_type == bbRU_avoidanceType_none) continue;

                I32 inner_int_i = inner_unit->md.coords.i;
                I32 inner_int_j = inner_unit->md.coords.j;
                I32 outer_int_i = outer_unit->md.coords.i;
                I32 outer_int_j = outer_unit->md.coords.j;

                I32 i_int_dif = inner_int_i - outer_int_i;
                I32 j_int_dif = inner_int_j - outer_int_j;

                float inner_float_i = inner_unit->correction.i;
                float inner_float_j = inner_unit->correction.j;
                float outer_float_i = outer_unit->correction.i;
                float outer_float_j = outer_unit->correction.j;

                float i_float_dif = inner_float_i - outer_float_i;
                float j_float_dif = inner_float_j - outer_float_j;

                float i_dif = i_int_dif + i_float_dif;
                float j_dif = j_int_dif + j_float_dif;

                float distance = sqrtf(i_dif * i_dif + j_dif * j_dif);

                float spacing = POINTS_PER_TILE;
                if (distance >= spacing) continue;
                float overlap = spacing - distance;

                outer_unit->forces.i -= overlap * i_dif / distance /3.0f;
                outer_unit->forces.j -= overlap * i_dif / distance /3.0f;


            }
        }
    } else {
        I32 i_inner = inner_group->units[0].owner->md.coords.i;
        I32 j_inner = inner_group->units[0].owner->md.coords.j;
        I32 i_outer = outer_group->units[0].owner->md.coords.i;
        I32 j_outer = outer_group->units[0].owner->md.coords.j;

        I32 i_dif = i_inner - i_outer;
        I32 j_dif = j_inner - j_outer;

        float distance = sqrtf(i_dif * i_dif + j_dif * j_dif);
        if (distance > POINTS_PER_TILE * 16) return bbContinue;

        for (I32 i = 0; i < UNITS_PER_GROUP; i++) {
            bbRenderUnit* outer_unit = &outer_group->units[i];
            if (outer_unit->avoidance_type == bbRU_avoidanceType_none) continue;
            if (outer_unit->avoidance_type == bbRU_avoidanceType_rigid) continue;
            for (I32 j = 0; j < UNITS_PER_GROUP; j++) {
                bbRenderUnit* inner_unit = &inner_group->units[j];
                if (inner_unit->avoidance_type == bbRU_avoidanceType_none) continue;

                I32 inner_int_i = inner_unit->md.coords.i;
                I32 inner_int_j = inner_unit->md.coords.j;
                I32 outer_int_i = outer_unit->md.coords.i;
                I32 outer_int_j = outer_unit->md.coords.j;

                I32 i_int_dif = inner_int_i - outer_int_i;
                I32 j_int_dif = inner_int_j - outer_int_j;

                float inner_float_i = inner_unit->correction.i;
                float inner_float_j = inner_unit->correction.j;
                float outer_float_i = outer_unit->correction.i;
                float outer_float_j = outer_unit->correction.j;

                float i_float_dif = inner_float_i - outer_float_i;
                float j_float_dif = inner_float_j - outer_float_j;

                float i_dif = i_int_dif + i_float_dif;
                float j_dif = j_int_dif + j_float_dif;

                float distance = sqrtf(i_dif * i_dif + j_dif * j_dif);

                float spacing = POINTS_PER_TILE;
                if (distance >= spacing) continue;
                float overlap = spacing - distance;

                outer_unit->forces.i -= overlap * i_dif / distance /3.0f;
                outer_unit->forces.j -= overlap * i_dif / distance /3.0f;



            }
        }
    }


    return bbContinue;
}

static bbFlag sum_forces_outer(bbList* list, void* node, void* cl) {

    bbIterator inner_iterator = bbIterator_new(list);
    bbIterator_mapL(&inner_iterator,sum_forces_inner,node);
    return bbContinue;
}

static bbFlag apply_correction(bbList* list, void* node, void* cl) {
    bbRenderUnitGroup* group = node;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++) {
        group->units[i].correction.i += group->units[i].forces.i;
        group->units[i].correction.j += group->units[i].forces.j;
        group->units[i].forces.i = 0;
        group->units[i].forces.j = 0;
    }
    return bbContinue;
}


static bbFlag update_position(bbList* list, void* node, void* cl) {
    bbRenderUnitGroup* group = node;

    for (I32 i = 0; i < UNITS_PER_GROUP; i++) {
        group->units[i].md.coords.i += group->units[i].correction.i;
        group->units[i].md.coords.j += group->units[i].correction.j;
        group->units[i].correction.i = 0;
        group->units[i].correction.j = 0;
    }
    return bbContinue;
}

bbFlag bbRenderUnits_avoidance(bbRenderUnits* render_units) {
    //reset positions
    bbRenderUnits_updateMovement(render_units);
    bbIterator outer_iterator = bbIterator_new(&render_units->list);

    for (I32 pass = 0; pass < AVOIDANCE_PASSES; pass++) {
        //sum forces
        bbIterator_mapL(&outer_iterator,sum_forces_outer,NULL);
        //apply correction
        bbIterator_mapL(&outer_iterator,apply_correction,NULL);

    }

    //update positions
    bbIterator_mapL(&outer_iterator,update_position,NULL);
    return bbSuccess;
}

#endif //BB_RENDER_UNITS_AVOIDANCE